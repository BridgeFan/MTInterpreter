//
// Created by kamil-hp on 04.01.2022.
//

#ifndef MTINTERPRETER_SEMANTICANALIZER_H
#define MTINTERPRETER_SEMANTICANALIZER_H
#include "Visitor.h"
#include "../SyntaxTree/InitNode.h"
#include <map>
#include <string>
#include <stack>
#include <vector>
#include "SemanticExpressionTree.h"

enum BlockEndMode: uint16_t {
	ContinueEnd,
	BreakEnd,
	NormalEnd
};

class SemanticAnalizer: public Visitor {
	std::string actualFunction;
	TypeType functionReturned;
	//name, depth, type
	std::map<std::string, std::map<int, TypeType> > vars;
	std::optional<SemanticExpressionTree> expressionTree;
	MappedSyntaxTree* syntaxTree;
	BlockEndMode blockEndMode=NormalEnd;
	int depth; //0-global, 1-parameters, 2+-function block
	void addDepth() {depth++;}
	void removeDepth();
	void addVar(const std::string& name, TypeType type);
	TypeType getVar(const std::string& name) const;
public:
	bool analize(MappedSyntaxTree& tree);
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
};


#endif //MTINTERPRETER_SEMANTICANALIZER_H
