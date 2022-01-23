//
// Created by kamil-hp on 04.01.2022.
//

#include <iostream>
#include "Interpreter.h"
#include "../SyntaxTree/MappedSyntaxTree.h"
#include "../SyntaxTree/AssignNode.h"
#include "../SyntaxTree/Parameter.h"
#include "../SyntaxTree/IfNode.h"
#include "../SyntaxTree/WhileNode.h"
#include "../SyntaxTree/ForNode.h"
#include "../SyntaxTree/ReturnNode.h"
#include "../SyntaxTree/FunCall.h"
#include "InterpreterExpressionTree.h"


void Interpreter::addVar(const std::string &name, int64_t value){
	actualScope->vars.insert({name, value});
}
void Interpreter::addVar(const std::string &name, double value){
	actualScope->vars.insert({name, value});
}


void Interpreter::setVar(const std::string &name, const InterpreterValue& value) {
	Scope* scope=actualScope;
	while(scope && scope->vars.find(name)==scope->vars.end()) {
		scope=scope->parent;
	}
	if(!scope && globalScope.vars.find(name)!=globalScope.vars.end()) {
		scope = &globalScope;
	}
	if(scope) {
		switch(scope->vars[name].index()) {
			case int_:
				if(value.index()==int_)
					scope->vars[name]=value;
				else
					throw std::runtime_error("Trying to set illegal type to int variable "+name);
				break;
			case double_:
				if(value.index()==int_)
					scope->vars[name]=(double)std::get<int_>(value);
				else if(value.index()==double_)
					scope->vars[name]=value;
				else
					throw std::runtime_error("Trying to set illegal type to int variable "+name);
				break;
			default:
				throw std::runtime_error("Trying to set to wrong type of variable "+name);
		}
	}
	else
		throw std::runtime_error("Trying to set unknown variable "+name);
}

InterpreterValue Interpreter::getVar(const std::string &name) const {
	const Scope* scope=actualScope;
	while(scope && scope->vars.find(name)==scope->vars.end()) {
		scope=scope->parent;
	}
	if(!scope && globalScope.vars.find(name)!=globalScope.vars.end()) {
		scope = &globalScope;
	}
	if(scope)
		return scope->vars.at(name);
	else
		throw std::runtime_error("Trying to get unknown variable " + name);
}

void Interpreter::beginScope() {
	actualScope = actualScope ? new Scope(*actualScope) : new Scope();
}

void Interpreter::endScope() {
	Scope* scope = actualScope;
	actualScope=actualScope->parent;
	delete scope;
}

void Interpreter::clearScope() {
	while(actualScope) {
		Scope* scope=actualScope->parent;
		delete actualScope;
		actualScope=scope;
	}
	actualScope=nullptr;
}

int64_t Interpreter::visitTree(MappedSyntaxTree &tree) {
	globalScope={};
	syntaxTree=&tree;
	for(auto&& [name, var]: tree.globalVars) {
		actualScope= new Scope;
		if(var.type==int_)
			addVar(name,(int64_t)0);
		else
			addVar(name,0.0);
		var.accept(*this);
	}
	tree.functions["main"].accept(*this);
	clearScope();
	globalScope={};
	return std::get<int_>(returnedValue);
}

void Interpreter::visit(AssignNode &node) {
	auto value = node.expression->calculate(*this);
	if(getVar(node.id).index()==int_ && value.index()==double_) {
		throw std::runtime_error("Trying to assign double to int");
	}
	auto var = getVar(node.id);
	switch(node.type) {
		case assign2:
			setVar(node.id, value);
			break;
		case addAssign2:
			setVar(node.id, getBinaryOperationValue(var,value,add));
			break;
		case minusAssign2:
			setVar(node.id, getBinaryOperationValue(var,value,minus));
			break;
		case multAssign2:
			setVar(node.id, getBinaryOperationValue(var,value,mult));
			break;
		case divAssign2:
			setVar(node.id, getBinaryOperationValue(var,value,divi));
			break;
		case moduloAssign2:
			setVar(node.id, getBinaryOperationValue(var,value,mod));
			break;
	}
}

void Interpreter::visit(Block &node) {
	beginScope();
	for(const auto& line: node.lines) {
		line->accept(*this);
		if(blockEndMode!=NormalEnd)
			break;
	}
	endScope();
}

void Interpreter::visit(Line &node) {
	node.accept(*this);
}

void Interpreter::visit(LoopModLine &node) {
	blockEndMode=(BlockEndMode)node.type;
}

void Interpreter::visit(Parameter &node) {
	node.accept(*this);
}

void Interpreter::visit(IfNode &node) {
	int64_t value = std::get<int_>(node.condition->calculate(*this));
	if(value!=0l)
		node.stat.accept(*this);
	else if(node.elseStat)
		node.elseStat->accept(*this);
}

void Interpreter::visit(InitNode &node) {
	for(auto& v: node.vars) {
		if(node.type==int_)
			addVar(v.first, (int64_t)0);
		else
			addVar(v.first, 0.0);
		if(!v.second)
			continue;
		auto val = v.second->calculate(*this);
		setVar(v.first, val);
	}
}

void Interpreter::visit(ReturnNode &node) {
	if(node.returnedValue)
		returnedValue=node.returnedValue->calculate(*this);
	else
		returnedValue=nullptr;
	blockEndMode=ReturnEnd;
}

void Interpreter::visit(WhileNode &node) {
	int64_t condition = std::get<int_>(node.condition->calculate(*this));
	while(condition!=0l) {
		node.stat.accept(*this);
		if(blockEndMode==BreakEnd || blockEndMode==ReturnEnd)
			break;
		condition = std::get<int_>(node.condition->calculate(*this));
	}
	if(blockEndMode!=ReturnEnd)
		blockEndMode=NormalEnd;
}

void Interpreter::visit(FunctionNode &node) {
	Scope* scope=actualScope;
	actualScope=new Scope();
	for(int i=0;i<node.parameters.size();i++) {
		if(node.parameters[i].type==double_ && paramValues[i].index()==int_)
			paramValues[i]=(double)std::get<int_>(paramValues[i]);
		if(paramValues[i].index()==int_)
			addVar(node.name,std::get<int_>(paramValues[i]));
		if(paramValues[i].index()==double_)
			addVar(node.name,std::get<double_>(paramValues[i]));
	}
	paramValues.clear();
	node.block.accept(*this);
	clearScope();
	actualScope=scope;
}

void Interpreter::visit(FunCall &node) {
	paramValues.clear();
	for (const auto &p: node.params) {
		paramValues.push_back(p->calculate(*this));
	}
	if (node.name == "scan") {
		if(!paramValues.empty())
			throw std::runtime_error("Wrong number of parameters of scan");
		int64_t a;
		in >> a;
		returnedValue=a;
	}
	else if(node.name=="scanf") {
		if(!paramValues.empty())
			throw std::runtime_error("Wrong number of parameters of scanf");
		double a;
		in >> a;
		returnedValue=a;
	}
	else if(node.name=="print") {
		if(paramValues.size()!=1)
			throw std::runtime_error("Wrong number of parameters of print");
		std::visit(overloaded {
			[&](int64_t a) {out << a;},
			[&](double a) {out << a;},
			[](std::nullptr_t a) {throw std::runtime_error("Unexpected void in print");},
			[&](const std::string& a) {out << a;},
		}, paramValues[0]);
		returnedValue=nullptr;
	}
	else
		syntaxTree->functions[node.name].accept(*this);
}

void Interpreter::visit(ForNode &node) {
	node.init.accept(*this);
	if(node.assignNodePre)
		node.assignNodePre->accept(*this);
	int64_t condition = std::get<int_>(node.condition->calculate(*this));
	while(condition!=0l) {
		node.stat.accept(*this);
		if(blockEndMode==BreakEnd || blockEndMode==ReturnEnd)
			break;
		if(node.assignNodeEach)
			node.assignNodeEach->accept(*this);
		condition = std::get<int_>(node.condition->calculate(*this));
	}
	if(blockEndMode!=ReturnEnd)
		blockEndMode=NormalEnd;

}

InterpreterValue Interpreter::calculate(StringExpression &node) {
	return node.value;
}

InterpreterValue Interpreter::calculate(NumberExpression &node) {
	return std::visit(overloaded {
		[](int64_t arg) {return (InterpreterValue)arg;},
		[](double arg) {return (InterpreterValue)arg;},
		},
		node.value);
}

InterpreterValue Interpreter::calculate(IdExpression &node) {
	auto var = getVar(node.value);
	if(var.index()==void_)
		throw(std::runtime_error("Unknown variable"));
	return var;
}

InterpreterValue Interpreter::calculate(Expression &node) {
	auto val1 = node.expression1->calculate(*this);
	if(node.expression2) {
		auto val2 = node.expression2->calculate(*this);
		return getBinaryOperationValue(val1, val2, node.op);
	}
	else
		return getUnaryOperationValue(val1, node.op);
}

InterpreterValue Interpreter::calculate(FunCallExpression &node) {
	node.funCall->accept(*this);
	return returnedValue;
}


