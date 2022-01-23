//
// Created by kamil-hp on 12.12.2021.
//

#ifndef MTINTERPRETER_SYNTAXTREE_H
#define MTINTERPRETER_SYNTAXTREE_H
#include <utility>
#include <vector>
#include "FunctionNode.h"
#include "InitNode.h"

struct SyntaxTree {
	std::vector<InitNode> globalVars;
	std::vector<FunctionNode> functions;
};

#endif //MTINTERPRETER_SYNTAXTREE_H
