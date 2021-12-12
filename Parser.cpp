//
// Created by kamil-hp on 10.12.2021.
//

#include "Parser.h"
#include "ErrorHandler.h"
#include "SyntaxTree/SyntaxTree.h"
#include "Token/Conversion.h"
#include <algorithm>
#include <utility>
#include <iostream>
#include <queue>
#include <stack>
#include <map>

const std::vector<TokenType> anyToken = {Id_,If_,Else_,While_,For_,LoopMod_,TypeName_,ParBegin_,ParEnd_,End_,Comma_,NegOp_,
		Minus_,	Add_,Return_,Assign_,MultOp_,RelOp_,BlockBegin_,BlockEnd_,Logic_,Conversion_,String_,Number_,Void_}; //all possible values expect Error_

Parser::Parser(Scaner& scaner): scaner(scaner), prevToken(nullptr) {
}

Parser::Parser(Scaner&& scaner): scaner(scaner), prevToken(nullptr) {
}

std::unique_ptr<Token> Parser::getScanerToken(std::vector<TokenType> allowedTypes) {
	std::unique_ptr<Token> token;
	wasWrongType=false;
	//allow EOF as UINT16_MAX
	if(prevToken)
		token = std::move(prevToken);
	else
		token = scaner.getNextToken();
	if(!token) {
		if(std::find(allowedTypes.begin(),allowedTypes.end(),_eof)!=allowedTypes.end())
			return nullptr;
		auto error = ErrorToken(scaner.getLine(),scaner.getColumn(),"",Error_,unexpectedEof);
		ErrorHandler::addError(ParserError,error);
		return nullptr;
	}
	if(token->getType()==Error_) {
		ErrorHandler::addError(ScanerError, *dynamic_cast<ErrorToken*>(token.get()));
		return nullptr;
	}
	if(std::find(allowedTypes.begin(),allowedTypes.end(),token->getType())==allowedTypes.end()) {
		ErrorHandler::addError(ParserError,ErrorToken(token->getLine(),token->getColumn(),token->getType(),allowedTypes[0]));
		wasWrongType=true;
		//return nullptr;
	}
	return token;
}

std::optional<Block> Parser::getBlock() {
	//assumes position after BlockBegin
	Block block;
	std::unique_ptr<Line> line;
	std::unique_ptr<Token> token;
	while((token=getScanerToken(anyToken)) && token->getType()!=BlockEnd_) {
		line=getLine();
		if(!line || typeid(*line)==typeid(Line)) {
			return block;
		}
		block.lines.emplace_back(std::move(line));
	}
	if(!token) {
		return block;
	}
	if(token->getType()!=BlockEnd_) {
		ErrorHandler::addError(ParserError,ErrorToken(token->getLine(),token->getColumn(),token->getType(),BlockEnd_));
	}
	return block;
}

std::unique_ptr<Line> Parser::getLine(std::unique_ptr<Token> token) {
	//for blocks it assumes position after {
	if(!token)
		token = getScanerToken({End_,If_,While_,For_,BlockBegin_,Return_,Id_,TypeName_,BlockEnd_});
	if(!token)
		return nullptr; //no line wasn't able to create
	if(wasWrongType)
		return nullptr; //line isn't able to create
	if(token->getType()==BlockEnd_) {
		return nullptr;
	}
	if(token->getType()==End_) {
		return std::make_unique<Line>();
	}
	if(token->getType()==If_) {
		auto val = getIf();
		return val ? std::make_unique<IfNode>(std::move(*val)) : nullptr;
	}
	if(token->getType()==While_) {
		auto val = getWhile();
		return val ? std::make_unique<WhileNode>(std::move(*val)) : nullptr;
	}
	if(token->getType()==For_) {
		auto val = getFor();
		return val ? std::make_unique<ForNode>(std::move(*val)) : nullptr;
	}
	if(token->getType()==BlockBegin_) {
		auto val = getBlock();
		return val ? std::make_unique<Block>(std::move(*val)) : nullptr;
	}
	if(token->getType()==Return_) {
		auto token2 = getScanerToken({Id_,ParBegin_,End_,NegOp_,Minus_,Add_,MultOp_,RelOp_,Logic_,Conversion_,String_,Number_});
		if(!token2)
			return std::make_unique<ReturnNode>();
		if(wasWrongType) {
			//assume that there should be end of line
			prevToken=std::move(token2);
			return std::make_unique<ReturnNode>();
		}
		if(token2->getType()==End_)
			return std::make_unique<ReturnNode>();
		auto expr = getExpression(End_,std::move(token2));
		if(!expr.first)
			return std::make_unique<ReturnNode>();
		return std::make_unique<ReturnNode>(*expr.first);
	}
	if(token->getType() == TypeName_) {
		auto val = getInit(std::unique_ptr<TypeName>(dynamic_cast<TypeName*>(token.release())));
		return val ? std::make_unique<InitNode>(std::move(*val)) : nullptr;
	}
	if(token->getType()==Id_) {
		auto token2 = getScanerToken({Assign_,ParBegin_});
		if(!token2)
			return nullptr;
		if(wasWrongType) //TODO: what to assume
			return nullptr;
		if(token2->getType()==Assign_) {
			std::vector<std::unique_ptr<Token> > assignTokens;
			assignTokens.emplace_back(std::move(token));
			assignTokens.emplace_back(std::move(token2));
			auto assignNode = getAssign(std::move(assignTokens));
			if(!assignNode)
				return nullptr;
			return std::make_unique<AssignNode>(std::move(*assignNode));
		}
		if(token2->getType()==ParBegin_) {
			auto val = getFunCall(std::unique_ptr<IdToken>(dynamic_cast<IdToken*>(token.release())));
			return val ? std::make_unique<FunCall>(std::move(*val)) : nullptr;
		}
	}
	return nullptr; //no recognized type of line
}

std::optional<IfNode> Parser::getIf() {
	//if↓
	auto token = getScanerToken(ParBegin_);
	if(!token)
		return std::nullopt;
	if(wasWrongType) { //assume that there is ParBegin
		prevToken=std::move(token);
	}
	auto expr = getExpression(ParEnd_);
	IfNode node;
	if(!expr.first)
		return std::nullopt;
	auto block = getLine();
	if(block) {
		if(typeid(*block)==typeid(Block))
			node.stat = *dynamic_cast<Block*>(block.release());
		else
			node.stat.lines.emplace_back(std::move(block));
	}
	else
		node.stat=Block();
	node.condition=std::move(expr.first);
	auto token3 = getScanerToken(anyToken);
	if(!token3)
		return node;
	if(token3->getType()==Else_) {
		auto elseBlock = getLine();
		if(elseBlock) {
			if(typeid(*block)==typeid(Block))
				node.stat = *dynamic_cast<Block*>(block.release());
			else
				node.stat.lines.emplace_back(std::move(block));
		}
		else
			node.elseStat=nullptr;
		return node;
	}
	else {
		prevToken = std::move(token3);
		node.elseStat=nullptr;
	}
	return node;
}

std::optional<WhileNode> Parser::getWhile() {
	//while↓
	auto token = getScanerToken(ParBegin_);
	if(!token)
		return std::nullopt;
	if(wasWrongType) //assume that there is ParBegin
		prevToken=std::move(token);
	auto expr = getExpression(ParEnd_);
	if(!expr.first)
		return std::nullopt;
	WhileNode node;
	auto block = getLine();
	if(block) {
		if(typeid(*block)==typeid(Block))
			node.stat = *dynamic_cast<Block*>(block.release());
		else
			node.stat.lines.emplace_back(std::move(block));
	}
	else
		node.stat=Block();
	node.condition=std::move(expr.first);
	return node;
}

std::optional<ForNode> Parser::getFor() {
	//for↓
	auto token = getScanerToken(ParBegin_);
	if(!token)
		return std::nullopt;
	if(wasWrongType) //assume that there is ParBegin
		prevToken=std::move(token);
	ForNode node;
	auto token2 = getScanerToken(anyToken);
	if(!token2)
		return std::nullopt;
	if(token2->getType()!=End_) {
		std::vector<std::unique_ptr<Token> > tokenTmp;
		tokenTmp.emplace_back(std::move(token2));
		auto assignNode = getAssign(std::move(tokenTmp));
		if(!assignNode)
			return std::nullopt;
		node.assignNodePre=std::move(assignNode);
	}
	auto expr = getExpression(End_);
	if (!expr.first)
		return std::nullopt;
	node.condition = std::move(expr.first);

	auto token3 = getScanerToken(anyToken);
	if(!token3)
		return std::nullopt;
	if(token3->getType()!=ParEnd_) {
		std::vector<std::unique_ptr<Token> > tokenTmp;
		tokenTmp.emplace_back(std::move(token3));
		auto assignNode = getAssign(std::move(tokenTmp));
		if(!assignNode)
			return std::nullopt;
		node.assignNodeEach=std::move(assignNode);
	}
	auto block = getLine();
	if(block) {
		if(typeid(*block)==typeid(Block))
			node.stat = *dynamic_cast<Block*>(block.release());
		else
			node.stat.lines.emplace_back(std::move(block));
	}
	else
		node.stat=Block();
	return node;
}

std::optional<InitNode> Parser::getInit(std::unique_ptr<TypeName> typeToken, std::unique_ptr<IdToken> idToken) {
	//int/double↓
	InitNode node;
	std::unique_ptr<Token> token;
	wasWrongType=false;
	if(!typeToken)
		token=getScanerToken(TypeName_);
	else
		token=std::move(typeToken);
	if(!token)
		return std::nullopt;
	if(wasWrongType) //assume that double is declared
		token=std::make_unique<TypeName>(-1,-1,TypeNameType::doubleType);
	node.type=*dynamic_cast<TypeName*>(token.get());
	while(token && token->getType()!=End_) {
		if(idToken==nullptr) {
			auto tmpToken = getScanerToken(Id_);
			if (!tmpToken)
				return std::nullopt;
			if (!wasWrongType)
				idToken = std::unique_ptr<IdToken>(dynamic_cast<IdToken*>(tmpToken.release()));
		}
		token=getScanerToken({End_, Comma_, Assign_});
		if(!token)
			return std::nullopt;
		if(wasWrongType) { //assume that there should be End
			prevToken=std::move(token);
			token=std::make_unique<Token>(End_,-1,-1);
		}
		if(token->getType()!=Assign_)
			node.vars.emplace_back(*idToken.release(),nullptr);
		else {
			auto tokenPtr = dynamic_cast<Assign*>(token.get());
			if(tokenPtr->getSubtype()!=AssignType::assign) {
				ErrorHandler::addError(ScanerError,ErrorToken(tokenPtr->getLine(), tokenPtr->getColumn(),tokenPtr->getSubtype()));
			}
			auto expr = getExpression({Comma_,End_});
			if(!expr.first)
				return std::nullopt;
			node.vars.emplace_back(*idToken.release(),std::move(expr.first));
			std::vector allowedTokensHere = anyToken;
			allowedTokensHere.push_back(_eof);
			token=std::move(prevToken);
		}
	}
	return node;
}

std::vector<TokenType> exprTokenTypes = {
		Id_,
		ParBegin_,
		ParEnd_,
		NegOp_,
		Minus_,
		Add_,
		MultOp_,
		RelOp_,
		Logic_,
		Conversion_,
		String_,
		Number_,
};

int getPriority(const std::pair<std::unique_ptr<Token>, bool>& val) {
	if(val.second)
		return 0;
	std::map<TokenType, int> opTokenPriorities = {
			{NegOp_, 1},
			{Conversion_, 1},
			{MultOp_, 2},
			{Add_, 3},
			{Minus_, 4},
			{Logic_, 5},
			{RelOp_, 6}
	};
	if(!val.first)
		return 9999;
	auto it = opTokenPriorities.find(val.first->getType());
	if(it==opTokenPriorities.end())
		return 9000;
	return it->second;
}

//all except ParEnd
std::vector<TokenType> exitTokenTypes = {
		End_,
		Comma_,
		//BlockEnd_=19u,
};

std::pair<std::unique_ptr<Expression>, std::unique_ptr<Token> > Parser::getExpression(const std::vector<TokenType>& allowedEnds, std::unique_ptr<Token> firstToken) {
	//TODO
	std::unique_ptr<Token> token;
	if(firstToken)
		token=std::move(firstToken);
	else
		token= getScanerToken(anyToken);
	//bool is true for unary operators
	typedef std::variant<std::unique_ptr<Expression>, std::pair<std::unique_ptr<Token>, bool> > QueueObj;
	std::queue<QueueObj> exit;
	std::stack<std::pair<std::unique_ptr<Token>, bool> > stack;
	bool isMinusUnary=true;
	//convert to Reverse Polish Notation
	while(token && std::find(exprTokenTypes.begin(),exprTokenTypes.end(),token->getType())!=exprTokenTypes.end()) {
		if(!token)
			break;
		if(token->getType()==String_) {
			std::unique_ptr<StringExpression> strExpr(new StringExpression());
			strExpr->token=*dynamic_cast<StringToken*>(token.release());
			exit.push(std::unique_ptr<StringExpression>(strExpr.release()));
			isMinusUnary=false;
			token = getScanerToken(anyToken);
			continue;
		}
		else if (token->getType()==Number_) {
			std::unique_ptr<NumberExpression> nrExpr(new NumberExpression());
			nrExpr->token=*dynamic_cast<Number*>(token.release());
			exit.push(std::unique_ptr<NumberExpression>(nrExpr.release()));
			isMinusUnary=false;
			token = getScanerToken(anyToken);
			continue;
		}
		else if(token->getType()==Id_) {
			auto token2 = getScanerToken(anyToken);
			isMinusUnary=false;
			if(!token2)
				break;
			//check if variable or function call
			if(token2->getType()==ParBegin_) {
				auto fun = getFunCall(std::unique_ptr<IdToken>(dynamic_cast<IdToken*>(token.release())));
				if(!fun)
					break;
				std::unique_ptr<FunCallExpression> fnExpr(new FunCallExpression());
				fnExpr->token=std::unique_ptr<FunCall>(dynamic_cast<FunCall*>(token.release()));
				exit.emplace(std::move(fnExpr));
			}
			else { //variable → to exit
				IdExpression* idExpr = new IdExpression();
				idExpr->token=*dynamic_cast<IdToken*>(token.release());
				exit.push(std::unique_ptr<IdExpression>(idExpr));
				prevToken=std::move(token2);
			}
		}
		else if(token->getType()==ParBegin_) {
			isMinusUnary=true;
			auto token2 = getScanerToken(anyToken);
			if(!token2)
				break;
			if(token2->getType()==TypeName_) {
				//create conversion token
				auto token3 = getScanerToken(ParEnd_);
				if(!token3)
					break;
				if(wasWrongType) {
					prevToken=std::move(token3);
				}
				auto token22 = std::unique_ptr<TypeName>(dynamic_cast<TypeName*>(token2.release()));
				if(token22->getSubtype()==TypeNameType::intType)
					prevToken=std::make_unique<Conversion>(token->getLine(),token->getColumn(),toInt);
				else
					prevToken=std::make_unique<Conversion>(token->getLine(),token->getColumn(),toDouble);
			}
			else {
				prevToken = std::move(token2);
				stack.emplace(std::move(token),true);
			}
		}
		else if(token->getType()==ParEnd_) {
			isMinusUnary=false;
			if(stack.empty()) {
				ErrorHandler::addError(ParserError,{token->getLine(),token->getColumn(),unexpectedParEnd});
			}
			while(!stack.empty() && stack.top().first->getType()!=ParBegin_) {
				exit.emplace(std::move(stack.top()));
				stack.pop();
			}
			if(stack.empty()) {
				ErrorHandler::addError(ParserError,{token->getLine(),token->getColumn(),unexpectedParEnd});
			}
			else
				stack.pop();
		}
		else {
			std::pair<std::unique_ptr<Token>, bool> tmpObj;
			if(token->getType()==NegOp_ || token->getType()==Conversion_ || (token->getType()==Minus_ && isMinusUnary))
				tmpObj.second=true;
			else {
				tmpObj.second = false;
				isMinusUnary=true;
			}
			tmpObj.first=std::move(token);
			int priority = getPriority(tmpObj);
			while(!stack.empty() && getPriority(stack.top())>=getPriority(tmpObj) && !(stack.top().second && tmpObj.second)) {
				exit.emplace(std::move(stack.top()));
				stack.pop();
			}
			stack.emplace(std::move(tmpObj));
			//operator
		}
		token = getScanerToken(anyToken);
		//token
	}
	while(!stack.empty()) {
		auto& top = stack.top();
		if(top.first->getType()==ParEnd_)
			ErrorHandler::addError(ParserError,ErrorToken(top.first->getLine(),top.first->getColumn(),unexpectedParEnd));
		else if(stack.top().first->getType()==ParBegin_)
			ErrorHandler::addError(ParserError,ErrorToken(top.first->getLine(),top.first->getColumn(),unexpectedParBegin));
		else
			exit.emplace(std::move(top));
		stack.pop();
	}
	if((std::find(allowedEnds.begin(),allowedEnds.end(),token->getType())==allowedEnds.end())) {
		ErrorHandler::addError(ParserError, ErrorToken(token->getLine(),token->getColumn(),token->getType(),allowedEnds[0]));
		prevToken=std::make_unique<Token>(*token);
	}
	//create expression tree from ReversePolishNotation
	std::stack<std::unique_ptr<Expression> > calcStack;
	while(!exit.empty()) {
		auto& front = exit.front();
		if(front.index()==0) {
			//simple expression (number, string, function call or variable)
			calcStack.emplace(std::move(std::get<0>(front)));
			exit.pop();
		}
		else {
			auto frontC = std::move(std::get<1>(front));
			exit.pop();
			if(calcStack.empty())
				return {nullptr, std::move(token)};
			auto front2 = std::move(calcStack.top());
			calcStack.pop();
			std::unique_ptr<Expression> newExpr(new Expression);
			if(!frontC.second) { //binary operator
				if(!calcStack.empty()) {
					auto front3 = std::move(calcStack.top());
					calcStack.pop();
					newExpr->expression1=std::move(front3);
					newExpr->expression2.emplace(std::move(front2));
				}
				else {
					newExpr->expression2=std::move(front2);
					ErrorHandler::addError(ParserError,ErrorToken(-1,-1,"",frontC.first->getType(),unexpectedEndOfExpression));
				}
			}
			else {
				newExpr->expression1=std::move(front2);
			}
			newExpr->op=std::move(frontC.first);
			calcStack.emplace(std::move(newExpr));
		}
	}
	return {std::move(calcStack.top()),std::move(token)};
}

std::optional<FunCall> Parser::getFunCall(std::unique_ptr<IdToken> funName) {
	//funName(↓
	FunCall call;
	call.name=std::move(funName);
	auto token = getScanerToken(anyToken);
	if(!token)
		return std::nullopt;
	while(token->getType()!=ParEnd_) {
		auto expr = getExpression({Comma_,ParEnd_},std::move(token));
		if(!expr.first) //assume that there is ParEnd
			return call;
		if(wasWrongType) {//assume that there is ParEnd
			prevToken=std::move(expr.second);
			return call;
		}
		token=std::move(expr.second);
		call.params.emplace_back(std::move(expr.first));
	}
	return call;
}

bool Parser::parseNext() {
	auto retTypeToken = getScanerToken({TypeName_,Void_,_eof});
	if(!retTypeToken)
		return ErrorHandler::getErrorSize()!=0;
	if(wasWrongType) {
		//ignore
		return EXIT_FAILURE;
	}
	auto funcName = getScanerToken(Id_);
	if(!funcName)
		return EXIT_FAILURE;
	if(wasWrongType) {
		funcName=nullptr;
	}
	auto token3 = getScanerToken({ParBegin_, Comma_, End_,Assign_});
	if(!token3)
		return EXIT_FAILURE;
	if(wasWrongType) { //assume that this is function
		prevToken=std::move(token3);
		token3=std::make_unique<Token>(ParBegin_,-1,-1);
	}
	if(token3->getType()==ParBegin_) {
		//parse function
		FunctionNode func;
		if(funcName)
			func.id=*(dynamic_cast<IdToken *>(funcName.release()));
		if(retTypeToken->getType()==TypeName_)
			func.returnedType = *(dynamic_cast<TypeName*>(retTypeToken.get()));
		else
			func.returnedType = *retTypeToken;
		//get parameters
		std::unique_ptr<Token> token;
		while ((token=getScanerToken({TypeName_, ParEnd_})) && token->getType() == TypeName_) {
			std::unique_ptr<Token> name = getScanerToken(Id_);
			if (!name)
				return EXIT_FAILURE;
			if(wasWrongType) {
				prevToken = std::move(name);
				name = std::make_unique<IdToken>(-1,-1,"");
			}
			func.parameters.emplace_back(*dynamic_cast<TypeName *>(token.get()),
			                             *dynamic_cast<IdToken *>(name.get()));
		}
		if (!token)
			return EXIT_FAILURE;
		auto block = getBlock();
		if (!block)
			return EXIT_FAILURE;
		func.block = *block;
		functions.emplace_back(std::move(func));
		return EXIT_SUCCESS;
	}
	else {
		//parse global variable
		std::vector<std::unique_ptr<Token> > tokens;
		if(retTypeToken->getType()==Void_) {
			ErrorHandler::addError(ParserError,ErrorToken(retTypeToken->getLine(),retTypeToken->getColumn(),retTypeToken->getType(),TypeName_));
			return EXIT_FAILURE;
		}
		prevToken = std::move(token3);
		auto node = getInit(std::unique_ptr<TypeName>(dynamic_cast<TypeName *>(retTypeToken.release())),
		                    funcName ? std::unique_ptr<IdToken>(dynamic_cast<IdToken *>(funcName.release())) : std::make_unique<IdToken>(-1,-1,""));
		if(!node)
			return EXIT_FAILURE;
		globalVars.push_back(*node);
		return EXIT_SUCCESS;
	}
}

SyntaxTree Parser::parse() {
	//create function
	while(!scaner.hasEnded() && parseNext()==EXIT_SUCCESS)
		;
	SyntaxTree tree;
	tree.globalVars=std::move(globalVars);
	globalVars.clear();
	tree.functions=std::move(functions);
	functions.clear();
	return tree;
}

std::optional<AssignNode> Parser::getAssign(std::vector<std::unique_ptr<Token>> tokens) {
	//id assignNode statement
	AssignNode assignNode;
	std::unique_ptr<Token> assignToken1, assignToken2;
	if(!tokens.empty()) {
		assignToken1 = std::move(tokens.back());
		tokens.pop_back();
	}
	else
		assignToken1 = getScanerToken(Id_);
	if (!assignToken1)
		return std::nullopt;
	if(wasWrongType) {
		prevToken = std::move(assignToken1);
		assignToken1 = std::make_unique<IdToken>(-1,-1,"");
	}
	assignNode.id = std::unique_ptr<IdToken>(dynamic_cast<IdToken *>(assignToken1.release()));

	if(!tokens.empty()) {
		assignToken2 = std::move(tokens.back());
		tokens.pop_back();
	}
	else
		assignToken2 = getScanerToken(Assign_);
	if (!assignToken2)
		return std::nullopt;
	if(wasWrongType) {
		prevToken = std::move(assignToken1);
		assignToken1 = std::make_unique<Assign>(-1,-1,AssignType::assign);
	}
	assignNode.assign = std::unique_ptr<Assign>(dynamic_cast<Assign *>(assignToken2.release()));


	auto expr = getExpression(End_);
	if (!expr.first) {
		return std::nullopt;
	}
	assignNode.expression = std::move(expr.first);

	return assignNode;
}
