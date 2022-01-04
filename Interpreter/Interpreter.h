//
// Created by kamil-hp on 04.01.2022.
//

#ifndef MTINTERPRETER_INTERPRETER_H
#define MTINTERPRETER_INTERPRETER_H

#include "Visitor.h"
#include "InterpreterExpressionTree.h"
#include "SemanticAnalizer.h"

class Interpreter: public Visitor {
	std::string actualFunction;
	TypeType functionReturned;
	//name, depth, type
	std::map<std::string, std::map<int, InterpreterValue> > vars;
	std::vector<InterpreterValue> paramValues;
	std::optional<InterpreterExpressionTree> expressionTree;
	MappedSyntaxTree* syntaxTree;
	BlockEndMode blockEndMode=NormalEnd;
	int depth; //0-global, 1-parameters, 2+-function block
	void addDepth() {depth++;}
	void removeDepth();
	void addVar(const std::string& name, int64_t value);
	void addVar(const std::string& name, double value);
	[[nodiscard]] InterpreterValue getVar(const std::string& name) const;
public:
	void visit(AssignNode& node) override;
	void visit(Block& node) override;
	void visit(Expression& node) override;
	void visit(IdExpression& node) override;
	void visit(StringExpression& node) override;
	void visit(NumberExpression& node) override;
	void visit(ForNode& node) override;
	void visit(FunCall& node) override;
	void visit(FunctionNode& node) override;
	void visit(IfNode& node) override;
	void visit(InitNode& node) override;
	void visit(ReturnNode& node) override;
	void visit(Parameter& node) override;
	void visit(Line& node) override;
	void visit(WhileNode& node) override;
	void visit(LoopModLine& node) override;
	bool visitTree(MappedSyntaxTree& tree) override;

};


#endif //MTINTERPRETER_INTERPRETER_H
