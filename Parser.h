//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_PARSER_H
#define MTINTERPRETER_PARSER_H

#include <optional>
#include "SyntaxTree/Parameter.h"
#include "SyntaxTree/Block.h"
#include "SyntaxTree/FunctionNode.h"
#include "SyntaxTree/IfNode.h"
#include "SyntaxTree/WhileNode.h"
#include "SyntaxTree/ForNode.h"
#include "SyntaxTree/FunCall.h"
#include "SyntaxTree/ReturnNode.h"
#include "Scaner.h"

class Parser {
	std::unique_ptr<Token> prevToken=nullptr;
	Scaner& scaner;
	std::vector<FunctionNode> functions;
	std::optional<Block> getBlock();
	std::optional<Line> getLine(std::unique_ptr<Token> token=nullptr);
	std::optional<IfNode> getIf();
	std::optional<WhileNode> getWhile();
	std::optional<ForNode> getFor();
	std::optional<FunCall> getFunCall(std::unique_ptr<IdToken> funName);
	std::optional<InitNode> getInit(std::unique_ptr<TypeName> typeToken);
	std::pair<std::optional<Expression>, std::unique_ptr<Token> > getExpression(std::unique_ptr<Token> firstToken=nullptr);
	std::unique_ptr<Token> getScanerToken(std::vector<TokenType> allowedTypes);
	std::unique_ptr<Token> getScanerToken(TokenType type) {return getScanerToken(std::vector({type}));}
public:
	bool parseNext();
	explicit Parser(Scaner& scaner);
	void parse();
};


#endif //MTINTERPRETER_PARSER_H
