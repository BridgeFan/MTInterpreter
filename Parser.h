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
#include "SyntaxTree/SyntaxTree.h"

class Parser {
	bool wasWrongType=false;
	std::unique_ptr<Token> prevToken=nullptr;
	Scaner& scaner;
	std::vector<FunctionNode> functions;
	std::vector<InitNode> globalVars;
	std::optional<AssignNode> getAssign(std::vector<std::unique_ptr<Token> > tokens={});
	std::optional<Block> getBlock();
	std::unique_ptr<Line> getLine(std::unique_ptr<Token> token=nullptr);
	std::optional<IfNode> getIf();
	std::optional<WhileNode> getWhile();
	std::optional<ForNode> getFor();
	std::optional<FunCall> getFunCall(std::unique_ptr<IdToken> funName);
	std::optional<InitNode> getInit(std::unique_ptr<TypeName> typeToken, std::unique_ptr<IdToken> idToken=nullptr);
	std::pair<std::unique_ptr<Expression>, std::unique_ptr<Token> > getExpression(const std::vector<TokenType>& allowedEnds, std::unique_ptr<Token> firstToken=nullptr);
	std::pair<std::unique_ptr<Expression>, std::unique_ptr<Token> > getExpression(TokenType allowedEnd, std::unique_ptr<Token> firstToken=nullptr) {return getExpression(std::vector({allowedEnd}),std::move(firstToken));}
	std::unique_ptr<Token> getScanerToken(std::vector<TokenType> allowedTypes);
	std::unique_ptr<Token> getScanerToken(TokenType type) {return getScanerToken(std::vector({type}));}
public:
	bool parseNext();
	explicit Parser(Scaner& scaner);
	explicit Parser(Scaner&& scaner);
	SyntaxTree parse();
};


#endif //MTINTERPRETER_PARSER_H
