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
		if(!expr.first)
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
			auto expr = getExpression(std::unique_ptr<Assign>(dynamic_cast<Assign*>(token2.release())));
			if(!expr.first)
				return std::nullopt;
			AssignNode node;
			node.id=std::unique_ptr<IdToken>(dynamic_cast<IdToken*>(token.release()));
			node.assign=std::unique_ptr<Assign>(dynamic_cast<Assign*>(token2.release()));
			node.expression=*expr.first;
			return node;
		}
		if(token2->getType()==ParBegin_) {
			return getFunCall(std::unique_ptr<IdToken>(dynamic_cast<IdToken*>(token.release())));
		}
	}
	return std::nullopt;
}

std::optional<IfNode> Parser::getIf() {
	//TODO
	//if↓
	return std::nullopt;
}

std::optional<WhileNode> Parser::getWhile() {
	//TODO
	//while↓
	return std::nullopt;
}

std::optional<ForNode> Parser::getFor() {
	//TODO
	//for↓

	return std::nullopt;
}

std::optional<InitNode> Parser::getInit(std::unique_ptr<TypeName> typeToken) {
	//TODO
	//int/double↓
	return std::nullopt;
}

std::pair<std::optional<Expression>, std::unique_ptr<Token> > Parser::getExpression(std::unique_ptr<Token> firstToken) {
	//TODO
	return {std::nullopt, nullptr};
}

std::optional<FunCall> Parser::getFunCall(std::unique_ptr<IdToken> funName) {
	//TODO
	//funName(↓
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
