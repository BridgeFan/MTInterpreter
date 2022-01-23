//
// Created by kamil-hp on 04.01.2022.
//

#ifndef MTINTERPRETER_VISITOR_H
#define MTINTERPRETER_VISITOR_H

#include <variant>
#include "../SyntaxTree/Expression.h"

class AssignNode;
class Block;
class Expression;
class IdExpression;
class StringExpression;
class NumberExpression;
class FunCallExpression;
class ForNode;
class FunCall;
class FunctionNode;
class IfNode;
class InitNode;
class ReturnNode;
class Parameter;
class Line;
class WhileNode;
class MappedSyntaxTree;
class LoopModLine;
enum TypeType: uint16_t;
typedef std::variant<int64_t, double, std::nullptr_t, std::string> InterpreterValue;

class Visitor {
public:
	virtual void visit(AssignNode& node) = 0;
	virtual void visit(Block& node) = 0;
	virtual void visit(ForNode& node) = 0;
	virtual void visit(FunCall& node) = 0;
	virtual void visit(FunctionNode& node) = 0;
	virtual void visit(IfNode& node) = 0;
	virtual void visit(InitNode& node) = 0;
	virtual void visit(ReturnNode& node) = 0;
	virtual void visit(Parameter& node) = 0;
	virtual void visit(Line& node) = 0;
	virtual void visit(WhileNode& node) = 0;
	virtual void visit(LoopModLine& node) = 0;
	virtual int64_t visitTree(MappedSyntaxTree& tree) = 0;
	//calculate
	virtual InterpreterValue calculate(Expression& node) = 0;
	virtual InterpreterValue calculate(IdExpression& node) = 0;
	virtual InterpreterValue calculate(StringExpression& node) = 0;
	virtual InterpreterValue calculate(NumberExpression& node) = 0;
	virtual InterpreterValue calculate(FunCallExpression& node) = 0;
};

#endif //MTINTERPRETER_VISITOR_H
