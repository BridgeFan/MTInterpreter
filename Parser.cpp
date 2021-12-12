//
// Created by kamil-hp on 10.12.2021.
//

#include "Parser.h"
#include "ErrorHandler.h"
#include "SyntaxTree/SyntaxTree.h"
#include <algorithm>
#include <utility>
#include <iostream>

const std::vector<TokenType> anyToken = {Id_,If_,Else_,While_,For_,LoopMod_,TypeName_,ParBegin_,ParEnd_,End_,Comma_,NegOp_,
		Minus_,	Add_,Return_,Assign_,MultOp_,RelOp_,BlockBegin_,BlockEnd_,Logic_,Conversion_,String_,Number_,Void_}; //all possible values expect Error_

Parser::Parser(Scaner& scaner): scaner(scaner), prevToken(nullptr) {
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
		std::cout << "-1\n";
		if(std::find(allowedTypes.begin(),allowedTypes.end(),_eof)!=allowedTypes.end())
			return nullptr;
		auto error = ErrorToken(scaner.getLine(),scaner.getColumn(),"",Error_,unexpectedEof);
		ErrorHandler::addError(ParserError,error);
		return nullptr;
	}
	std::cout << token->getType() << "\n";
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
	Block block;
	std::unique_ptr<Line> line;
	std::unique_ptr<Token> token;
	while((token=getScanerToken(anyToken)) && token->getType()!=BlockEnd_) {
		line=getLine();
		if(!line) {
			return block;
		}
		block.lines.emplace_back(*line);
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
	if(!token)
		token = getScanerToken({End_,If_,While_,For_,BlockBegin_,Return_,Id_,TypeName_,BlockEnd_});
	if(!token)
		return nullptr; //no line wasn't able to create
	if(wasWrongType)
		return nullptr; //line isn't able to create
	if(token->getType()==BlockEnd_) {
		prevToken=std::move(token);
		return std::make_unique<Line>();
	}
	if(token->getType()==End_) {
		return std::unique_ptr(getLine());
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
			node.stat.lines.emplace_back(*block);
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
				node.stat.lines.emplace_back(*block);
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
			node.stat.lines.emplace_back(*block);
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
			node.stat.lines.emplace_back(*block);
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
	while(token->getType()!=End_) {
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
			node.vars.emplace_back(*idToken.release(),std::nullopt);
		else {
			auto tokenPtr = dynamic_cast<Assign*>(token.get());
			if(tokenPtr->getSubtype()!=AssignType::assign) {
				ErrorHandler::addError(ScanerError,ErrorToken(tokenPtr->getLine(), tokenPtr->getColumn(),tokenPtr->getSubtype()));
			}
			auto expr = getExpression({Comma_,End_});
			if(!expr.first)
				return std::nullopt;
			node.vars.emplace_back(*idToken.release(),*expr.first);
		}

	}
	return node;
}

std::pair<std::unique_ptr<Expression>, std::unique_ptr<Token> > Parser::getExpression(std::vector<TokenType> allowedEnds, std::unique_ptr<Token> firstToken) {
	//TODO
	return {nullptr, nullptr};
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
		call.params.emplace_back(*(expr.first));
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
	auto token3 = getScanerToken({ParBegin_, Comma_, End_});
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
