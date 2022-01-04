//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_PARSER_H
#define MTINTERPRETER_PARSER_H

#include <optional>
#include <queue>
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

enum ErrorType: uint16_t;
enum ParserState: uint16_t {
	correct=0u,
	finished=1u,
	error=2u
};

class Parser {
	std::deque<std::unique_ptr<Token> > prevTokens;
	Scaner& scaner;
	std::vector<FunctionNode> functions;
	std::vector<InitNode> globalVars;
	std::optional<AssignNode> getAssign();
	std::optional<Block> getBlock();
	std::unique_ptr<Line> getLine();
	std::optional<IfNode> getIf();
	std::optional<WhileNode> getWhile();
	std::optional<ForNode> getFor();
	std::optional<FunctionNode> getFunction();
	std::pair<bool, std::vector<Parameter> > getParameters();
	std::optional<FunCall> getFunCall();
	std::optional<InitNode> getInit();
	std::optional<ReturnNode> getReturn();
	std::pair<std::unique_ptr<Expression>, std::unique_ptr<Token> > getExpression();
	std::unique_ptr<Token> getNextToken();
	void addError(const std::unique_ptr<Token>& token, ErrorType type, std::vector<TokenType> allowedTypes={});
	ErrorType checkToken(const std::unique_ptr<Token>& token, std::vector<TokenType> allowedTypes);
	ErrorType checkToken(const std::unique_ptr<Token>& token, TokenType type) {return checkToken(token, std::vector({type}));}
public:
	ParserState parseNext();
	explicit Parser(Scaner& scaner);
	explicit Parser(Scaner&& scaner);
	SyntaxTree parse();
};


#endif //MTINTERPRETER_PARSER_H
