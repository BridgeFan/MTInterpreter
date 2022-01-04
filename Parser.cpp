//
// Created by kamil-hp on 10.12.2021.
//

#include "Parser.h"
#include "ErrorHandler.h"
#include "SyntaxTree/SyntaxTree.h"
#include "Token/Conversion.h"
#include "Token/StringToken.h"
#include "Token/TypeName.h"
#include "Token/IdToken.h"
#include "Token/MultOp.h"
#include "Token/RelOp.h"
#include "Token/LogicOp.h"
#include <algorithm>
#include <utility>
#include <iostream>
#include <queue>
#include <stack>
#include <map>

const std::vector<TokenType> anyToken = {Id_,If_,Else_,While_,For_,LoopMod_,TypeName_,ParBegin_,ParEnd_,End_,Comma_,NegOp_,
		Minus_,	Add_,Return_,Assign_,MultOp_,RelOp_,BlockBegin_,BlockEnd_,Logic_,Conversion_,String_,Number_,Void_}; //all possible values expect Error_ and EndOfFile

Parser::Parser(Scaner& scaner): scaner(scaner) {
}

Parser::Parser(Scaner&& scaner): scaner(scaner) {
}

std::unique_ptr<Token> Parser::getNextToken() {
	if(!prevTokens.empty()) {
		auto token = std::move(prevTokens.front());
		prevTokens.pop_front();
		return token;
	}
	else
		return scaner.getNextToken();
}

ErrorType Parser::checkToken(const std::unique_ptr<Token>& token, std::vector<TokenType> allowedTypes) {
	//returns noError=0 (false) if token is ok and positive value if token is wrong
	if(token->getType()==Error_) {
		ErrorHandler::addError(ScanerError, *dynamic_cast<ErrorToken*>(token.get()));
		return errorToken;
	}
	if(std::find(allowedTypes.begin(),allowedTypes.end(),token->getType())==allowedTypes.end()) {
		return wrongToken;
	}
	return noError;
}

void Parser::addError(const std::unique_ptr<Token>& token, ErrorType type, std::vector<TokenType> allowedTypes) {
	switch(type) {
		case noError:
			break;
		case wrongToken:
			ErrorHandler::addError(ParserError,ErrorToken(token->getLine(),token->getColumn(),token->getType(),allowedTypes[0]));
			break;
		case errorToken:
			//error already added by getNextToken()
			break;
		default: //TODO
			break;
	}

}

std::optional<Block> Parser::getBlock() {
	auto beginToken = getNextToken();
	if(checkToken(beginToken, BlockBegin_)!=noError) {
		prevTokens.push_front(std::move(beginToken));
		return std::nullopt;
	}
	Block block;
	std::unique_ptr<Line> line;
	std::unique_ptr<Token> token;
	while((token=getNextToken()) && checkToken(token, {BlockEnd_,EndOfFile})==wrongToken) {
		prevTokens.push_front(std::move(token));
		line=getLine();
		if(!line) {
			return block;
		}
		block.lines.emplace_back(std::move(line));
	}
	if(!token) {
		return block;
	}
	if(token->getType()!=BlockEnd_) {
		addError(token, wrongToken, {BlockEnd_});
	}
	return block;
}

std::unique_ptr<Line> Parser::getLine() {
	auto token = getNextToken();
	if(token->getType()==BlockEnd_) {
		return nullptr;
	}
	if(token->getType()==End_) {
		return std::make_unique<Line>();
	}
	prevTokens.push_front(std::move(token));
	auto valIf = getIf();
	if(valIf) {
		return std::make_unique<IfNode>(std::move(*valIf));
	}
	auto valWhile = getWhile();
	if(valWhile) {
		return std::make_unique<WhileNode>(std::move(*valWhile));
	}
	auto valFor = getFor();
	if(valFor) {
		return std::make_unique<ForNode>(std::move(*valFor));
	}
	auto valBlock = getBlock();
	if(valBlock) {
		return std::make_unique<Block>(std::move(*valBlock));
	}
	auto valInit = getInit();
	if(valInit) {
		return std::make_unique<InitNode>(std::move(*valInit));
	}
	auto valAssign = getAssign();
	if(valAssign) {
		return std::make_unique<AssignNode>(std::move(*valAssign));
	}
	auto valFunCall = getFunCall();
	if(valFunCall) {
		return std::make_unique<FunCall>(std::move(*valFunCall));
	}
	auto retCall = getReturn();
	if(retCall) {
		return std::make_unique<ReturnNode>(std::move(*retCall));
	}
	return nullptr; //no recognized type of line
}

std::optional<ReturnNode> Parser::getReturn() {
	auto retToken = getNextToken();
	ErrorType errorType;
	if ((errorType = checkToken(retToken, Return_)) != noError) {
		prevTokens.push_front(std::move(retToken));
		return std::nullopt;
	}
	//now we're sure that this is Return
	auto token2 = getNextToken();
	if(token2->getType()==End_)
		return ReturnNode();
	prevTokens.push_front(std::move(token2));
	auto expr = getExpression();
	if (!expr.first) {
		return std::nullopt;
	}
	if(expr.second->getType()!=End_) {
		addError(expr.second,wrongToken,{End_});
		prevTokens.push_front(std::move(expr.second));
	}
	ReturnNode node;
	node.returnedValue=std::move(expr.first);
	return node;
}

std::optional<IfNode> Parser::getIf() {
	auto ifToken = getNextToken();
	ErrorType errorType;
	if ((errorType=checkToken(ifToken, If_))!=noError) {
		prevTokens.push_front(std::move(ifToken));
		return std::nullopt;
	}
	//we are sure now that there should be if
	auto token = getNextToken();
	if((errorType=checkToken(token, ParBegin_))!=noError) {//assume that there is ParBegin
		prevTokens.push_front(std::move(token));
	}
	auto expr = getExpression();
	if (!expr.first) {
		return std::nullopt;
	}
	if(expr.second->getType()!=ParEnd_) { //assume that ther is ParEnd
		addError(expr.second,wrongToken,{ParEnd_});
		prevTokens.push_front(std::move(expr.second));
	}
	IfNode node;
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
	auto token3 = getNextToken();
	if(token3->getType()==Else_) {
		auto elseBlock = getLine();
		if(elseBlock) {
			if(typeid(*elseBlock)==typeid(Block))
				node.elseStat = std::move(*dynamic_cast<Block*>(elseBlock.release()));
			else {
				Block block2;
				block2.lines.emplace_back(std::move(elseBlock));
				node.elseStat=block2;
			}
		}
		else
			node.elseStat=std::nullopt;
		return node;
	}
	else {
		prevTokens.push_front(std::move(token3));
		node.elseStat=std::nullopt;
	}
	return node;
}

std::optional<WhileNode> Parser::getWhile() {
	auto ifToken = getNextToken();
	ErrorType errorType;
	if ((errorType=checkToken(ifToken, While_))!=noError) {
		prevTokens.push_front(std::move(ifToken));
		return std::nullopt;
	}
	//we are sure now that there should be while
	auto token = getNextToken();
	if((errorType=checkToken(token, ParBegin_))!=noError) {//assume that there is ParBegin
		addError(token, errorType, {ParBegin_});
		prevTokens.push_front(std::move(token));
	}
	auto expr = getExpression();
	if (!expr.first) {
		return std::nullopt;
	}
	if(expr.second->getType()!=ParEnd_) {
		addError(expr.second,wrongToken,{ParEnd_});
		prevTokens.push_front(std::move(expr.second));
	}
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
	auto forToken = getNextToken();
	ErrorType errorType;
	if ((errorType=checkToken(forToken, For_))!=noError) {
		prevTokens.push_front(std::move(forToken));
		return std::nullopt;
	}
	//we are sure now that there should be for
	auto token = getNextToken();
	if ((errorType=checkToken(token, ParBegin_))!=noError) {
		addError(token, errorType, {ParBegin_});
		prevTokens.push_front(std::move(token));
	}
	ForNode node;
	auto token2 = getNextToken();
	if(token2->getType()==Error_)
		return std::nullopt;
	if(token2->getType()!=End_) {
		prevTokens.push_front(std::move(token2));
		auto assignNode = getAssign();
		if(!assignNode)
			return std::nullopt;
		node.assignNodePre=std::move(assignNode);
	}
	auto expr = getExpression();
	if (!expr.first) {
		return std::nullopt;
	}
	if(expr.second->getType()!=ParEnd_) {
		addError(expr.second,wrongToken,{End_});
		prevTokens.push_front(std::move(expr.second));
	}
	node.condition = std::move(expr.first);

	auto token3 = getNextToken();
	if(!token3)
		return std::nullopt;
	if(token3->getType()!=ParEnd_) {
		prevTokens.push_front(std::move(token3));
		auto assignNode = getAssign();
		if(!assignNode)
			return std::nullopt;
		node.assignNodeEach=std::move(assignNode);
	}
	auto line = getLine();
	if(line) {
		if(typeid(*line)==typeid(Block))
			node.stat = *dynamic_cast<Block*>(line.release());
		else
			node.stat.lines.emplace_back(std::move(line));
	}
	else
		node.stat=Block();
	return node;
}

std::optional<InitNode> Parser::getInit() {
	auto token = getNextToken();
	ErrorType errorType = checkToken(token, TypeName_);
	if(errorType!=noError) {
		prevTokens.push_front(std::move(token));
		return std::nullopt;
	}
	std::unique_ptr<Token> token2;
	InitNode node;
	node.type=(TypeType)dynamic_cast<TypeName *>(token.get())->getSubtype();
	bool wasAnyVar=false;
	while(token->getType()!=End_ && token->getType()!=EndOfFile) {
		auto token3 = std::move(token);
		token2 = getNextToken();
		errorType=checkToken(token2,Id_);
		if(errorType!=noError) {
			prevTokens.push_front(std::move(token2));
			addError(token2, errorType, {Id_});
			break;
		}
		token = getNextToken();
		errorType=checkToken(token,{End_, Comma_, Assign_});
		if(errorType!=noError) {
			if(wasAnyVar) { //assume that there should be end of line
				addError(token, errorType, {End_, Comma_, Assign_});
				prevTokens.push_front(std::move(token));
				break;
			}
			else { //it can be function node
				prevTokens.push_front(std::move(token));
				prevTokens.push_front(std::move(token2));
				prevTokens.push_front(std::move(token3));
				return std::nullopt;
			}
		}
		wasAnyVar=true;
		if(token->getType()!=Assign_)
			node.vars.emplace_back(dynamic_cast<IdToken*>(token2.get())->getValue(),nullptr);
		else {
			auto tokenPtr = dynamic_cast<Assign*>(token.get());
			if(tokenPtr->getSubtype()!=AssignType::assign) {
				ErrorHandler::addError(ScanerError,ErrorToken(tokenPtr->getLine(), tokenPtr->getColumn(),tokenPtr->getSubtype()));
			}
			auto expr = getExpression();
			if (!expr.first) {
				break;
			}
			if(checkToken(expr.second,{Comma_, End_})!=noError) {
				addError(expr.second,wrongToken,{Comma_, End_});
				prevTokens.push_front(std::move(expr.second));
			}
			node.vars.emplace_back(dynamic_cast<IdToken*>(token2.get())->getValue(),std::move(expr.first));
			token=getNextToken();
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

std::pair<std::unique_ptr<Expression>, std::unique_ptr<Token> > Parser::getExpression() {
	std::unique_ptr<Token> token;
	token=getNextToken();
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
			strExpr->value=dynamic_cast<StringToken*>(token.get())->getValue();
			exit.push(std::unique_ptr<StringExpression>(strExpr.release()));
			isMinusUnary=false;
			token=getNextToken();
			continue;
		}
		else if (token->getType()==Number_) {
			std::unique_ptr<NumberExpression> nrExpr(new NumberExpression());
			nrExpr->value=dynamic_cast<Number*>(token.get())->getValue();
			exit.push(std::unique_ptr<NumberExpression>(nrExpr.release()));
			isMinusUnary=false;
			token=getNextToken();
			continue;
		}
		else if(token->getType()==Id_) {
			//auto token2 = getNextToken();
			isMinusUnary=false;
			/*if(token2->getType()==EndOfFile || token2->getType()==Error_)
				break;*/
			prevTokens.push_front(std::move(token));
			auto fun = getFunCall();
			if(fun) {
				//function call
				std::unique_ptr<FunCallExpression> fnExpr(new FunCallExpression());
				fnExpr->token=std::unique_ptr<FunCall>(dynamic_cast<FunCall*>(token.release()));
				exit.emplace(std::move(fnExpr));
			}
			else {
				token=getNextToken();
				//id expression
				auto* idExpr = new IdExpression();
				idExpr->value=dynamic_cast<IdToken*>(token.get())->getValue();
				exit.push(std::unique_ptr<IdExpression>(idExpr));
			}
		}
		else if(token->getType()==ParBegin_) {
			isMinusUnary=true;
			auto token2 = getNextToken();
			if(token2->getType()==Error_)
				break;
			if(token2->getType()==TypeName_) {
				//create conversion token
				auto token3 = getNextToken();
				ErrorType token3Error = checkToken(token3, ParEnd_);
				if(token3Error!=noError) {
					addError(token3,token3Error, {ParEnd_});
					prevTokens.push_front(std::move(token3));
				}
				auto token22 = std::unique_ptr<TypeName>(dynamic_cast<TypeName*>(token2.release()));
				if(token22->getSubtype()==TypeNameType::intType)
					prevTokens.push_front(std::make_unique<Conversion>(token->getLine(),token->getColumn(),toInt));
				else
					prevTokens.push_front(std::make_unique<Conversion>(token->getLine(),token->getColumn(),toDouble));
			}
			else {
				prevTokens.push_front(std::move(token2));
				stack.emplace(std::move(token),true);
			}
		}
		else if(token->getType()==ParEnd_) {
			isMinusUnary=false;
			if(stack.empty()) {
				break;
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
			while(!stack.empty() && getPriority(stack.top())<=getPriority(tmpObj) && !(stack.top().second && tmpObj.second)) {
				exit.emplace(std::move(stack.top()));
				stack.pop();
			}
			stack.emplace(std::move(tmpObj));
			//operator
		}
		token = getNextToken();
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
					newExpr->expression2=std::move(front2);
				}
				else {
					newExpr->expression2=std::move(front2);
					ErrorHandler::addError(ParserError,ErrorToken(-1,-1,"",frontC.first->getType(),unexpectedEndOfExpression));
				}
			}
			else {
				newExpr->expression1=std::move(front2);
			}
			switch(frontC.first->getType()) {
			case NegOp_:
				newExpr->op=negation;
				break;
			case Minus_:
				newExpr->op=minus;
				break;
			case Add_:
				newExpr->op=add;
				break;
			case MultOp_:
				newExpr->op=(OperatorType)(mult+dynamic_cast<MultOp*>(frontC.first.get())->getSubtype());
				break;
			case RelOp_:
				newExpr->op=(OperatorType)(eq+dynamic_cast<RelOp*>(frontC.first.get())->getSubtype());
				break;
			case Logic_:
				newExpr->op=(OperatorType)(Or+dynamic_cast<LogicOp*>(frontC.first.get())->getSubtype());
				break;
			case Conversion_:
				newExpr->op=(OperatorType)(toIntConversion+dynamic_cast<Conversion*>(frontC.first.get())->getSubtype());
				break;
			default:
				newExpr->op=none;
			}
			calcStack.emplace(std::move(newExpr));
		}
	}
	if(calcStack.empty())
		return {nullptr,std::move(token)};
	return {std::move(calcStack.top()),std::move(token)};
}

std::optional<FunCall> Parser::getFunCall() {
	auto funName = getNextToken();
	ErrorType errorType;
	if ((errorType=checkToken(funName, Id_))!=noError) {
		prevTokens.push_front(std::move(funName));
		return std::nullopt;
	}
	auto parBeginToken = getNextToken();
	if ((errorType=checkToken(parBeginToken, ParBegin_))!=noError) {
		prevTokens.push_front(std::move(parBeginToken));
		prevTokens.push_front(std::move(funName));
		return std::nullopt;
	}
	//now we are sure that this is function call
	FunCall call;
	call.name=dynamic_cast<IdToken*>(funName.get())->getValue();
	auto token = getNextToken();
	if(!token)
		return std::nullopt;
	while(token->getType()!=ParEnd_) {
		prevTokens.push_front(std::move(token));
		auto expr = getExpression();
		if (!expr.first) {
			return std::nullopt;
		}
		if(checkToken(expr.second,{Comma_, ParEnd_})!=noError) {
			addError(expr.second,wrongToken,{Comma_, ParEnd_});
			prevTokens.push_front(std::move(expr.second));
		}
		token=std::move(expr.second);
		call.params.emplace_back(std::move(expr.first));
	}
	return call;
}

ParserState Parser::parseNext() {
	if(scaner.hasEnded())
		return finished;
	auto globalVar = getInit();
	if(globalVar) {
		globalVars.push_back(std::move(*globalVar));
		return correct;
	}
	auto func = getFunction();
	if(func) {
		functions.push_back(std::move(*func));
		return correct;
	}
	//check end of file
	auto token = scaner.getNextToken();
	if(token->getType()==EndOfFile)
		return finished;
	prevTokens.push_front(std::move(token));
	return error;
}

SyntaxTree Parser::parse() {
	//create function
	while(!scaner.hasEnded() && parseNext()==correct)
		;
	SyntaxTree tree;
	tree.globalVars=std::move(globalVars);
	globalVars.clear();
	tree.functions=std::move(functions);
	functions.clear();
	return tree;
}

std::optional<AssignNode> Parser::getAssign() {
	//id assignNode statement
	AssignNode assignNode;
	std::unique_ptr<Token> assignToken1, assignToken2;
	assignToken1 = getNextToken();
	ErrorType errorType;
	if ((errorType=checkToken(assignToken1, Id_))!=noError) {
		prevTokens.push_front(std::move(assignToken1));
		return std::nullopt;
	}
	assignNode.id = dynamic_cast<IdToken *>(assignToken1.get())->getValue();
	assignToken2 = getNextToken();
	if ((errorType=checkToken(assignToken2, Assign_))!=noError) {
		prevTokens.push_front(std::move(assignToken2));
		prevTokens.push_front(std::move(assignToken1));
		return std::nullopt;
	}
	assignNode.type = (AssignNodeType)dynamic_cast<Assign *>(assignToken2.get())->getSubtype();

	auto expr = getExpression();
	if (!expr.first) {
		return std::nullopt;
	}
	if(expr.second->getType()!=End_) {
		addError(expr.second,wrongToken,{End_});
		prevTokens.push_front(std::move(expr.second));
	}
	assignNode.expression = std::move(expr.first);
	return assignNode;
}

std::optional<FunctionNode> Parser::getFunction() {
	auto typeToken = getNextToken();
	if(checkToken(typeToken, {TypeName_, Void_})!=noError) {
		prevTokens.push_front(std::move(typeToken));
		return std::nullopt;
	}
	auto idToken = getNextToken();
	if(checkToken(idToken, Id_)!=noError) {
		prevTokens.push_front(std::move(idToken));
		prevTokens.push_front(std::move(typeToken));
		return std::nullopt;
	}
	auto params = getParameters();
	if(!params.first) {
		prevTokens.push_front(std::move(idToken));
		prevTokens.push_front(std::move(typeToken));
		return std::nullopt;
	}
	//(_return_type | void) id Parameters Block
	FunctionNode func;
	if(typeToken->getType()==TypeName_) {
		func.returnedType=(TypeType)dynamic_cast<TypeName*>(typeToken.get())->getSubtype();
	}
	else
		func.returnedType=void_;
	func.name=dynamic_cast<IdToken*>(idToken.get())->getValue();
	func.parameters = std::move(params.second);
	auto block = getBlock();
	if (!block)
		return std::nullopt;
	func.block = *block;
	return func;
}

std::pair<bool, std::vector<Parameter> > Parser::getParameters() {
	auto beginToken = getNextToken();
	if(checkToken(beginToken, ParBegin_)!=noError) {
		prevTokens.push_front(std::move(beginToken));
		return {false, {}};
	}
	std::unique_ptr<Token> token;
	std::vector<Parameter> params;
	ErrorType errorType;
	while ((token=getNextToken())->getType()!=EndOfFile && checkToken(token, TypeName_)==noError) {
		std::unique_ptr<Token> name = getNextToken();
		errorType = checkToken(name, Id_);
		if (errorType!=noError) {
			addError(name, errorType, {Id_});
			if(errorType==wrongToken)
				prevTokens.push_front(std::move(name));
			name = std::make_unique<IdToken>(-1,-1,"");
		}
		params.emplace_back((TypeType)(dynamic_cast<TypeName *>(token.get())->getSubtype()),
		                             dynamic_cast<IdToken *>(name.get())->getValue());
	}
	if(token->getType()!=ParEnd_) {
		addError(token, errorType, {ParEnd_});
	}
	return {true, params};
}
