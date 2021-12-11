//
// Created by kamil-hp on 10.12.2021.
//

#include "Parser.h"
#include "ErrorHandler.h"
#include <algorithm>
#include <utility>
#include <iostream>

const std::vector<TokenType> anyToken = {Id_,If_,Else_,While_,For_,LoopMod_,TypeName_,ParBegin_,ParEnd_,End_,Comma_,NegOp_,
		Minus_,	Add_,Return_,Assign_,MultOp_,RelOp_,BlockBegin_,BlockEnd_,Logic_,Conversion_,String_,Number_,Void_}; //all possible values expect Error_

Parser::Parser(Scaner& scaner): scaner(scaner), prevToken(nullptr) {
}

std::unique_ptr<Token> Parser::getScanerToken(std::vector<TokenType> allowedTypes) {
	std::unique_ptr<Token> token;
	//allow EOF as UINT16_MAX
	if(prevToken)
		token = std::move(prevToken);
	else
		token = scaner.getNextToken();
	if(!token) {
		if(std::find(allowedTypes.begin(),allowedTypes.end(),UINT16_MAX)!=allowedTypes.end())
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
		ErrorHandler::addError(ParserError,ErrorToken(token->getLine(),token->getColumn(),"",allowedTypes[0],unexpectedToken));
		return nullptr;
	}
	return token;
}

std::optional<Block> Parser::getBlock() {
	Block block;
	std::optional<Line> line;
	std::unique_ptr<Token> token;
	while((token=getScanerToken(anyToken)) && token->getType()!=BlockEnd_) {
		line=getLine();
		if(!line) {
			return std::nullopt;
		}
		block.lines.emplace_back(*line);
	}
	if(!token) {
		return std::nullopt;
	}
	if(token->getType()!=BlockEnd_) {
		ErrorHandler::addError(ParserError,ErrorToken(token->getLine(),token->getColumn(),"",BlockEnd_,unexpectedToken));
		return std::nullopt;
	}
	return block;
}

std::optional<Line> Parser::getLine(std::unique_ptr<Token> token) {
	if(!token)
		token = getScanerToken({End_,If_,While_,For_,BlockBegin_,Return_,Id_,TypeName_});
	if(!token)
		return std::nullopt;
	if(token->getType()==End_) {
		return getLine();
	}
	if(token->getType()==If_) {
		return getIf();
	}
	if(token->getType()==While_) {
		return getWhile();
	}
	if(token->getType()==For_) {
		return getFor();
	}
	if(token->getType()==BlockBegin_) {
		return getBlock();
	}
	if(token->getType()==Return_) {
		auto token2 = getScanerToken({Id_,ParBegin_,End_,NegOp_,Minus_,Add_,MultOp_,RelOp_,Logic_,Conversion_,String_,Number_});
		if(!token2)
			return std::nullopt;
		if(token2->getType()==End_)
			return ReturnNode();
		auto expr = getExpression(std::move(token2));
		if(!expr.first || expr.second->getType()!=End_)
			return std::nullopt;
		return ReturnNode(*expr.first);
	}
	if(token->getType() == TypeName_) {
		return getInit(std::unique_ptr<TypeName>(dynamic_cast<TypeName*>(token.release())));
	}
	if(token->getType()==Id_) {
		auto token2 = getScanerToken({Assign_,ParBegin_});
		if(!token2)
			return std::nullopt;
		if(token2->getType()==Assign_) {
			std::vector<std::unique_ptr<Token> > assignTokens;
			assignTokens.emplace_back(std::move(token));
			assignTokens.emplace_back(std::move(token2));
			auto assignNode = getAssign(std::move(assignTokens));
			if(!assignNode)
				return std::nullopt;
			return assignNode;
		}
		if(token2->getType()==ParBegin_) {
			return getFunCall(std::unique_ptr<IdToken>(dynamic_cast<IdToken*>(token.release())));
		}
	}
	return std::nullopt;
}

std::optional<IfNode> Parser::getIf() {
	//if↓
	auto token = getScanerToken(ParBegin_);
	if(!token)
		return std::nullopt;
	auto expr = getExpression();
	if(!expr.first || expr.second->getType()!=ParEnd_)
		return std::nullopt;
	auto block = getBlock();
	if(!block)
		return std::nullopt;
	IfNode node;
	node.condition=std::move(expr.first);
	node.stat=*block;
	auto token3 = getScanerToken(anyToken);
	if(!token3)
		return std::nullopt;
	if(token3->getType()==Else_) {
		auto elseBlock = getBlock();
		if(!elseBlock)
			return std::nullopt;
		node.elseStat=*elseBlock;
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
	auto expr = getExpression();
	if(!expr.first || expr.second->getType()!=ParEnd_)
		return std::nullopt;
	auto block = getBlock();
	if(!block)
		return std::nullopt;
	WhileNode node;
	node.condition=std::move(expr.first);
	node.stat=*block;
	return node;
}

std::optional<ForNode> Parser::getFor() {
	//for↓
	auto token = getScanerToken(ParBegin_);
	if(!token)
		return std::nullopt;
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
	auto expr = getExpression();
	if (!expr.first || expr.second->getType() != End_)
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
	auto block = getBlock();
	if(!block)
		return std::nullopt;
	node.stat=block;
	return node;
}

std::optional<InitNode> Parser::getInit(std::unique_ptr<TypeName> typeToken) {
	//TODO: init value init
	//int/double↓
	InitNode node;
	std::unique_ptr<Token> token;
	if(!typeToken)
		token=getScanerToken(TypeName_);
	else
		token=std::move(typeToken);
	if(!token)
		return std::nullopt;
	node.type.reset(dynamic_cast<TypeName*>(token.release()));
	while(token->getType()!=End_) {
		auto token2 = getScanerToken(Id_);
		if(!token2)
			return std::nullopt;
		node.name.push_back(*dynamic_cast<IdToken*>(token2.get()));
		token=getScanerToken({End_, Comma_});
		if(!token)
			return std::nullopt;
	}
	return node;
}

std::pair<std::unique_ptr<Expression>, std::unique_ptr<Token> > Parser::getExpression(std::unique_ptr<Token> firstToken) {
	//TODO
	return {nullptr, nullptr};
}

std::optional<FunCall> Parser::getFunCall(std::unique_ptr<IdToken> funName) {
	//funName(↓
	FunCall call;
	call.name=std::move(funName);
	auto token = getScanerToken(anyToken);
	while(token->getType()!=ParEnd_) {
		auto expr = getExpression(std::move(token));
		if(!expr.first || (expr.second->getType()!=Comma_ && expr.second->getType()!=ParEnd_))
			return std::nullopt;
		token=std::move(expr.second);
	}

	return std::nullopt;
}

bool Parser::parseNext() {
	auto retTypeToken = getScanerToken({TypeName_,Void_,_eof});
	if(!retTypeToken)
		return ErrorHandler::getErrorSize()!=0;
	auto funcName = getScanerToken(Id_);
	if(!funcName)
		return EXIT_FAILURE;
	auto token3 = getScanerToken({ParBegin_});
	if(!token3)
		return EXIT_FAILURE;
	std::cout << "Ok\n";
	if(token3->getType()==ParBegin_) {
		//parse function
		FunctionNode func;
		func.id.reset(dynamic_cast<IdToken *>(funcName.release()));
		func.returnedType = std::move(retTypeToken);
		std::cout << "Ok\n";
		//get parameters
		std::unique_ptr<Token> token;
		while ((token = getScanerToken({TypeName_, ParEnd_})) && token->getType() == TypeName_) {
			std::cout << "param\n";
			std::unique_ptr<Token> name = getScanerToken(Id_);
			if (!name)
				return EXIT_FAILURE;
			func.parameters.emplace_back(std::unique_ptr<TypeName>(dynamic_cast<TypeName *>(token.release())),
			                             std::unique_ptr<IdToken>(dynamic_cast<IdToken *>(name.release())));
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
		//init global variable
		return EXIT_FAILURE;
	}
}

void Parser::parse() {
	//create function
	while(!scaner.hasEnded() && parseNext()==EXIT_SUCCESS)
		;

}

std::optional<AssignNode> Parser::getAssign(std::vector<std::unique_ptr<Token>> tokens) {
	//id assignNode statement
	AssignNode assignNode;
	std::unique_ptr<Token> assToken1, assToken2;
	if(!tokens.empty()) {
		assToken1 = std::move(tokens.back());
		tokens.pop_back();
	}
	else
		assToken1 = getScanerToken(Id_);
	if (!assToken1)
		return std::nullopt;
	assignNode.id = std::unique_ptr<IdToken>(dynamic_cast<IdToken *>(assToken1.release()));

	if(!tokens.empty()) {
		assToken2 = std::move(tokens.back());
		tokens.pop_back();
	}
	else
		assToken2 = getScanerToken(Assign_);
	if (!assToken2)
		return std::nullopt;
	assignNode.assign = std::unique_ptr<Assign>(dynamic_cast<Assign *>(assToken2.release()));


	auto expr = getExpression();
	if (!expr.first || expr.second->getType() != ParEnd_) {
		ErrorHandler::addError(ParserError,ErrorToken(-1,-1,"",ParEnd_,wrongEnd));
		return std::nullopt;
	}
	assignNode.expression = std::move(expr.first);

	return assignNode;
}
