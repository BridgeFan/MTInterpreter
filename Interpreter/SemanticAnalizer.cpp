//
// Created by kamil-hp on 04.01.2022.
//

#include "SemanticAnalizer.h"
#include "../SyntaxTree/MappedSyntaxTree.h"
#include "../SyntaxTree/Block.h"
#include "../SyntaxTree/Parameter.h"
#include "../SyntaxTree/AssignNode.h"
#include "../SyntaxTree/ReturnNode.h"
#include "../SyntaxTree/IfNode.h"
#include "../SyntaxTree/ForNode.h"
#include "../SyntaxTree/WhileNode.h"
#include "../SyntaxTree/FunCall.h"

const std::array<std::string, 3> MappedSyntaxTree::illegalNames = {"print","scan", "scanf"};

std::string toString(TypeType type) {
	switch(type) {
		case int_:
			return "int";
		case double_:
			return "double";
		case void_:
			return "void";
		case string_:
			return "string";
	}
	return "";
}

bool SemanticAnalizer::visitTree(MappedSyntaxTree &tree) {
	depth=0;
	syntaxTree=&tree;
	for(auto&& [name, var]: tree.globalVars) {
		var.accept(*this);
	}
	for(auto&& [name, func]: tree.functions) {
		if(std::find(MappedSyntaxTree::illegalNames.begin(),MappedSyntaxTree::illegalNames.end(),name)!=MappedSyntaxTree::illegalNames.end())
			continue;
		actualFunction=name;
		func.accept(*this);
	}
	syntaxTree=nullptr;
	return ErrorHandler::getErrorSize()==0;
}

void SemanticAnalizer::visit(FunctionNode &node) {
	addDepth();
	for(auto& p: node.parameters)
		visit(p);
	functionReturned = node.returnedType;
	node.block.accept(*this);
	if(blockEndMode!=NormalEnd) {
		ErrorHandler::addUncoveredLoopMod(actualFunction,(LoopModT)blockEndMode);
	}
	blockEndMode=NormalEnd;
	removeDepth();
}

void SemanticAnalizer::visit(Block &node) {
	addDepth();
	for(auto& l: node.lines) {
		l->accept(*this);
	}
	removeDepth();
}

void SemanticAnalizer::visit(Line &node) {
	node.accept(*this);
}

void SemanticAnalizer::visit(Parameter &node) {
	addVar(node.name, node.type);
}

void SemanticAnalizer::addVar(const std::string &name, TypeType type) {
	if(vars.find(name)==vars.end())
		vars[name]={};
	if(!vars[name].empty() && vars[name].rbegin()->first==depth) {
		//already declared at the same level
		ErrorHandler::addSemanticError( AlreadyDeclaredVariable,actualFunction,name);
		return;
	}
	vars[name][depth]=type;
}

TypeType SemanticAnalizer::getVar(const std::string &name) const {
	if(vars.find(name)==vars.end() || vars.at(name).empty()) {
		ErrorHandler::addSemanticError(NotDeclaredVariable,actualFunction,name);
		return void_;
	}
	return vars.at(name).rbegin()->second;
}

void SemanticAnalizer::visit(AssignNode &node) {
	TypeType varType = getVar(node.id);
	node.expression->accept(*this);
	if(!expressionTree) {
		//should be checked in expression visit
		return;
	}
	if((expressionTree->type==double_ && varType==int_) || expressionTree->type==string_) {
		ErrorHandler::addWrongAssignError(actualFunction, node.id, varType, expressionTree->type);
	}
	expressionTree=std::nullopt;
}

void SemanticAnalizer::removeDepth() {
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

void SemanticAnalizer::visit(ReturnNode &node) {
	if(!node.returnedValue) {
		if (functionReturned != void_)
			ErrorHandler::addWrongReturnError(actualFunction,functionReturned, void_);
		return;
	}
	node.returnedValue->accept(*this);
	if((expressionTree->type==int_ && functionReturned!=int_) || expressionTree->type==string_) {
		ErrorHandler::addWrongReturnError(actualFunction,functionReturned, expressionTree->type);
	}
	expressionTree=std::nullopt;
}

bool isConvertible(TypeType from, TypeType to) {
	if(from==to)
		return true;
	if(from==void_ || to==void_)
		return false;
	if(to==string_)
		return true;
	if(from==string_)
		return false;
	return from==int_ && to==double_;
}

void SemanticAnalizer::visit(FunCall &node) {
	if(!syntaxTree || syntaxTree->functions.find(node.name)==syntaxTree->functions.end()) {
		ErrorHandler::addSemanticError(NotDeclaredFunction,actualFunction,node.name);
		return;
	}
	const auto& funParams = syntaxTree->functions[node.name].parameters;
	if(node.params.size()!=funParams.size()) {
		ErrorHandler::addWrongParameterError(actualFunction,node.name,funParams.size(),node.params.size());
		return;
	}
	for(int i=0;i<node.params.size();i++) {
		node.params[i]->accept(*this);
		if(!isConvertible(expressionTree->type,funParams[i].type)) {
			ErrorHandler::addWrongParameterError(actualFunction,node.name,i,funParams[i].type,expressionTree->type);
		}
		expressionTree=std::nullopt;
	}
}


void SemanticAnalizer::visit(StringExpression &node) {
	if(!expressionTree)
		expressionTree={string_,nullptr};
	else
		expressionTree->type=string_;
}

void SemanticAnalizer::visit(Expression &node) {
	if(!expressionTree) { //none is initialized
		expressionTree={void_,nullptr};
	}
	expressionTree->setLeft(void_);
	if(node.expression1) {
		expressionTree=*expressionTree->getLeft();
		node.expression1->accept(*this);
		expressionTree=*expressionTree->getParent();
	}
	TypeType type2=void_;
	if(node.expression2) {
		expressionTree=*expressionTree->getRight();
		node.expression2->accept(*this);
		expressionTree=*expressionTree->getParent();
		type2 = expressionTree->getRight()->type;
	}
	auto type1 = expressionTree->getLeft()->type;
	if(type1==string_ || type2==string_) {
		expressionTree->type=string_;
		if(node.op!=add) {
			if(type2==void_)
				ErrorHandler::addIllegalOperationError(actualFunction,node.op,type1);
			else
				ErrorHandler::addIllegalOperationError(actualFunction,node.op,type1,type2);
		}
		expressionTree->unsetLeft();
		expressionTree->unsetRight();
		return;
	}
	//number operations
	switch(node.op) {
		case minus:
			if(type2==void_) {
				expressionTree->type=type1;
				break;
			}
		case add:
		case mult:
		case divi:
		case mod:
			if(type1==double_ || type2==double_)
				expressionTree->type=double_;
			else if(type1==int_ || type2==int_)
				expressionTree->type=int_;
			break;
		case eq:
		case neq:
		case mor:
		case meq:
		case les:
		case leq:
			expressionTree->type=int_;
			break;
		case negation:
			if(type1!=int_)
				ErrorHandler::addIllegalOperationError(actualFunction,node.op,type1);
			expressionTree->type=int_;
			break;
		case Or:
		case And:
			if(type1==double_ || type2==double_)
				ErrorHandler::addIllegalOperationError(actualFunction,node.op,type1, type2);
			expressionTree->type=int_;
			break;
		case toIntConversion:
			expressionTree->type=int_;
		case toDoubleConversion:
			expressionTree->type=double_;
		case none:
			break;
	}
	expressionTree->unsetLeft();
	expressionTree->unsetRight();
}

void SemanticAnalizer::visit(NumberExpression &node) {
	TypeType type;
	if(node.value.index()==0)
		type=int_;
	else
		type=double_;
	if(!expressionTree)
		expressionTree={type,nullptr};
	else
		expressionTree->type=type;
}

void SemanticAnalizer::visit(IdExpression &node) {
	TypeType type=getVar(node.value);
	if(!expressionTree)
		expressionTree={type,nullptr};
	else
		expressionTree->type=type;
}

void SemanticAnalizer::visit(IfNode &node) {
	node.condition->accept(*this);
	if(expressionTree->type!=int_) {
		ErrorHandler::addWrongConditionError(actualFunction,"if",expressionTree->type);
	}
	expressionTree=std::nullopt;
	node.stat.accept(*this);
	if(node.elseStat)
		node.elseStat->accept(*this);
}

void SemanticAnalizer::visit(WhileNode &node) {
	node.condition->accept(*this);
	if(expressionTree->type!=int_) {
		ErrorHandler::addWrongConditionError(actualFunction,"while",expressionTree->type);
	}
	expressionTree=std::nullopt;
	node.stat.accept(*this);
	blockEndMode=NormalEnd;
}

void SemanticAnalizer::visit(ForNode &node) {
	node.condition->accept(*this);
	if(expressionTree->type!=int_) {
		ErrorHandler::addWrongConditionError(actualFunction,"for",expressionTree->type);
	}
	expressionTree=std::nullopt;
	node.init.accept(*this);
	if(node.assignNodePre)
		node.assignNodePre->accept(*this);
	if(node.assignNodeEach)
		node.assignNodeEach->accept(*this);
	node.stat.accept(*this);
	blockEndMode=NormalEnd;
}

void SemanticAnalizer::visit(InitNode &node) {
	for(auto& v: node.vars) {
		addVar(v.first, node.type);
		if(!v.second)
			continue;
		v.second->accept(*this);
		if(expressionTree->type!=node.type) {
			ErrorHandler::addWrongAssignError(actualFunction, v.first, node.type, expressionTree->type);
		}
		expressionTree=std::nullopt;
	}
}

void SemanticAnalizer::visit(LoopModLine &node) {
	blockEndMode=(BlockEndMode)node.type;
}
