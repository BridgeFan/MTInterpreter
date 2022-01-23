//
// Created by kamil-hp on 04.01.2022.
//

#ifndef MTINTERPRETER_INTERPRETER_H
#define MTINTERPRETER_INTERPRETER_H

#include "Visitor.h"
#include "InterpreterExpressionTree.h"
#include <map>

enum BlockEndMode: uint16_t {
	ContinueEnd=0u,
	BreakEnd=1u,
	NormalEnd=2u,
	ReturnEnd=3u
};

struct Scope {
	std::map<std::string, InterpreterValue> vars;
	Scope* parent;
	Scope()=default;
	Scope(Scope& parent): parent(&parent) {}
};

class Interpreter: public Visitor {
	InterpreterValue returnedValue;
	Scope* actualScope=nullptr;
	Scope globalScope;
	std::vector<InterpreterValue> paramValues;
	MappedSyntaxTree *syntaxTree=nullptr;
	BlockEndMode blockEndMode=NormalEnd;
	std::ostream& out;
	std::istream& in;
	void beginScope();
	void endScope();
	void clearScope();
	void addVar(const std::string& name, int64_t value);
	void addVar(const std::string& name, double value);
	void setVar(const std::string& name, const InterpreterValue& value);
	[[nodiscard]] InterpreterValue getVar(const std::string& name) const;
public:
	Interpreter(std::istream& in, std::ostream& out): in(in), out(out) {}
	void visit(AssignNode& node) override;
	void visit(Block& node) override;
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
	int64_t visitTree(MappedSyntaxTree& tree) override;
	InterpreterValue calculate(Expression& node) override;
	InterpreterValue calculate(IdExpression& node) override;
	InterpreterValue calculate(StringExpression& node) override;
	InterpreterValue calculate(NumberExpression& node) override;
	InterpreterValue calculate(FunCallExpression& node) override;
	~Interpreter() {clearScope();}
};


#endif //MTINTERPRETER_INTERPRETER_H
