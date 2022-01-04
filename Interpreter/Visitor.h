//
// Created by kamil-hp on 04.01.2022.
//

#ifndef MTINTERPRETER_VISITOR_H
#define MTINTERPRETER_VISITOR_H

class AssignNode;
class Block;
class Expression;
class IdExpression;
class StringExpression;
class NumberExpression;
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

class Visitor {
public:
	virtual void visit(AssignNode& node) = 0;
	virtual void visit(Block& node) = 0;
	virtual void visit(Expression& node) = 0;
	virtual void visit(IdExpression& node) = 0;
	virtual void visit(StringExpression& node) = 0;
	virtual void visit(NumberExpression& node) = 0;
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
};

#endif //MTINTERPRETER_VISITOR_H
