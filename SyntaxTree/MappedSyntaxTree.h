//
// Created by kamil-hp on 04.01.2022.
//

#ifndef MTINTERPRETER_MAPPEDSYNTAXTREE_H
#define MTINTERPRETER_MAPPEDSYNTAXTREE_H
#include "SyntaxTree.h"
#include "../ErrorHandler.h"
#include <map>
#include <functional>

class MappedSyntaxTree {
public:
	static const std::array<std::string, 3> illegalNames;
	std::map<std::string, InitNode> globalVars;
	std::map<std::string, FunctionNode> functions;
	bool mapTree(const SyntaxTree& tree) {
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

};

#endif //MTINTERPRETER_MAPPEDSYNTAXTREE_H
