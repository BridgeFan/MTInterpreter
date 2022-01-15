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

void Interpreter::addVar(const std::string &name, int64_t value){
	vars[name][depth]=std::move(value);
}
void Interpreter::addVar(const std::string &name, double value){
	vars[name][depth]=std::move(value);
}

InterpreterValue Interpreter::getVar(const std::string &name) const {
	return vars.at(name).rbegin()->second;
}

void Interpreter::removeDepth() {
	depth--;
	for(auto&& [name, v]: vars) {
		if(v.empty()) {
			vars.erase(name);
			continue;
		}
		if(v.rbegin()->first>depth) {
			v.erase(v.rbegin()->first);
		}
		if(v.empty())
			vars.erase(name);
	}
}

bool Interpreter::visitTree(MappedSyntaxTree &tree) {
	depth=0;
	syntaxTree=&tree;
	for(auto&& [name, var]: tree.globalVars) {
		vars[name]={};
		if(var.type==int_)
			vars[name][0]=0ll;
		else
			vars[name][0]=0.0;
		var.accept(*this);
		if(expressionTree) {
			vars[name][0]=expressionTree->value;
		}
		expressionTree=std::nullopt;
	}
	tree.functions["main"].accept(*this);
	std::cout << "Main function result was " << std::get<int_>(expressionTree->value) << "\n";
	return ErrorHandler::getErrorSize()==0;
}

void Interpreter::visit(AssignNode &node) {
	node.expression->accept(*this);
	if(vars[node.id].rbegin()->second.index()==double_ && expressionTree->value.index()==int_) {
		expressionTree->value=(double)(std::get<int_>(expressionTree->value));
	}
	auto& var = vars[node.id].rbegin()->second;
	switch(node.type) {
		case assign2:
			var=expressionTree->value;
		case addAssign2:
			var=getBinaryOperationValue(var,expressionTree->value,add);
		case minusAssign2:
			var=getBinaryOperationValue(var,expressionTree->value,minus);
		case multAssign2:
			var=getBinaryOperationValue(var,expressionTree->value,mult);
		case divAssign2:
			var=getBinaryOperationValue(var,expressionTree->value,divi);
		case moduloAssign2:
			var=getBinaryOperationValue(var,expressionTree->value,mod);
	}
	vars[node.id].rbegin()->second=expressionTree->value;
	expressionTree=std::nullopt;
}

void Interpreter::visit(Block &node) {
	addDepth();
	for(auto line: node.lines) {
		line->accept(*this);
		if(blockEndMode!=NormalEnd)
			break;
	}
	removeDepth();
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
	node.condition->accept(*this);
	if(std::get<int_>(expressionTree->value)!=0l) {
		expressionTree=std::nullopt;
		node.stat.accept(*this);
	}
	else if(node.elseStat) {
		expressionTree=std::nullopt;
		node.elseStat->accept(*this);
	}
}

void Interpreter::visit(InitNode &node) {
	for(auto& v: node.vars) {
		if(node.type==int_)
			addVar(v.first, (int64_t)0);
		else
			addVar(v.first, 0.0);
		if(!v.second)
			continue;
		v.second->accept(*this);
		if(expressionTree) {
			if(expressionTree->value.index()==double_ && vars[v.first][depth].index()==int_)
				vars[v.first][depth]=(int64_t)std::get<double_>(expressionTree->value);
			else
				vars[v.first][depth]=expressionTree->value;
		}
		expressionTree=std::nullopt;
	}
}

void Interpreter::visit(ReturnNode &node) {
	if(node.returnedValue)
		node.returnedValue->accept(*this);
	blockEndMode=ReturnEnd;
}

void Interpreter::visit(WhileNode &node) {
	node.condition->accept(*this);
	while(std::get<int_>(expressionTree->value)!=0l) {
		expressionTree=std::nullopt;
		node.stat.accept(*this);
		if(blockEndMode==BreakEnd || blockEndMode==ReturnEnd)
			break;
	}
	if(blockEndMode!=ReturnEnd)
		blockEndMode=NormalEnd;
	expressionTree=std::nullopt;
}

void Interpreter::visit(FunctionNode &node) {
	addDepth();
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
	removeDepth();
}

void Interpreter::visit(FunCall &node) {
	for (const auto &p: node.params) {
		p->accept(*this);
		paramValues.push_back(expressionTree->value);
		expressionTree = std::nullopt;
	}
	if (node.name == "scan") {
		int64_t a;
		std::cin >> a;
		NumberExpression number;
		number.value=a;
		number.accept(*this);
	}
	else if(node.name=="scanf") {
		double a;
		std::cin >> a;
		NumberExpression number;
		number.value=a;
		number.accept(*this);
	}
	else if(node.name=="print") {
		switch(paramValues[0].index()) {
			case TypeType::int_:
				std::cout << std::get<int_>(paramValues[0]);
				break;
			case TypeType::double_:
				std::cout << std::get<double_>(paramValues[0]);
				break;
			case TypeType::void_:
				break;
			case TypeType::string_:
				std::cout << std::get<string_>(paramValues[0]);
				break;
		}

	}
	else
		syntaxTree->functions[node.name].accept(*this);
}

void Interpreter::visit(ForNode &node) {
	node.init.accept(*this);
	if(node.assignNodePre)
		node.assignNodePre->accept(*this);
	node.condition->accept(*this);
	while(std::get<int_>(expressionTree->value)!=0l) {
		expressionTree=std::nullopt;
		node.stat.accept(*this);
		if(blockEndMode==BreakEnd || blockEndMode==ReturnEnd)
			break;
		if(node.assignNodeEach)
			node.assignNodeEach->accept(*this);
	}
	if(blockEndMode!=ReturnEnd)
		blockEndMode=NormalEnd;
	expressionTree=std::nullopt;

}

void Interpreter::visit(StringExpression &node) {
	if(!expressionTree)
		expressionTree={string_,nullptr};
	expressionTree->value=node.value;
}

void Interpreter::visit(NumberExpression &node) {
	InterpreterValue value;
	if(!expressionTree)
		expressionTree={0,nullptr};
	if(node.value.index()==0)
		expressionTree->value=std::get<0>(node.value);
	else
		expressionTree->value=std::get<1>(node.value);
}

void Interpreter::visit(IdExpression &node) {
	if(!expressionTree)
		expressionTree={string_,nullptr};
	expressionTree->value=getVar(node.value);
}

void Interpreter::visit(Expression &node) {
	if(!expressionTree) { //none is initialized
		expressionTree={void_,nullptr};
	}
	expressionTree->setLeft(void_);
	expressionTree=*expressionTree->getLeft();
	node.expression1->accept(*this);
	expressionTree=*expressionTree->getParent();
	if(node.expression2) {
		expressionTree->setRight(void_);
		expressionTree=*expressionTree->getRight();
		node.expression2->accept(*this);
		expressionTree=*expressionTree->getParent();
		expressionTree->value=getBinaryOperationValue(expressionTree->getLeft()->value,expressionTree->getRight()->value, node.op);
	}
	else {
		expressionTree->value= getUnaryOperationValue(expressionTree->getLeft()->value,node.op);
	}
	expressionTree->unsetLeft();
	expressionTree->unsetRight();

}
