//
// Created by kamil-hp on 23.01.2022.
//
#include "MappedSyntaxTree.h"
const std::array<std::string, 3> MappedSyntaxTree::illegalNames = {"scan, scanf, print"};

bool MappedSyntaxTree::mapTree(const SyntaxTree& tree) {
	bool isCorrect=true;
	//true if correct
	for(const auto& init: tree.globalVars) {
		for(const auto& v: init.vars) {
			if(globalVars.find(v.first)!=globalVars.end()) {
				ErrorHandler::addSemanticError(AlreadyDeclaredVariable, v.first);
				isCorrect=false;
				continue;
			}
			InitNode initNode;
			initNode.type=init.type;
			initNode.vars.emplace_back(v);
			globalVars[v.first]=initNode;
		}
	}
	for(const auto& func: tree.functions) {
		if(functions.find(func.name)!=functions.end()) {
			ErrorHandler::addSemanticError(AlreadyDeclaredFunction, func.name);
			isCorrect=false;
			continue;
		}
		if(std::find(illegalNames.begin(),illegalNames.end(), func.name)!=illegalNames.end()) {
			ErrorHandler::addSemanticError(IllegalFunctionName, func.name);
			isCorrect=false;
			continue;
		}
		functions[func.name]=func;
	}
	//check main (should be int main() {})
	if(functions.find("main")==functions.end()) {
		ErrorHandler::addSemanticError(WrongMain, "");
		return false;
	}
	const auto& main = functions["main"];
	if(!main.parameters.empty() || main.returnedType!=TypeType::int_) {
		ErrorHandler::addSemanticError(WrongMain, "");
		return false;
	}
	functions["scan"]=FunctionNode("scan",int_);
	functions["scanf"]=FunctionNode("scanf",double_);
	functions["print"]=FunctionNode("print",void_);
	functions["print"].parameters.emplace_back(string_, "a");
	return isCorrect;
}
