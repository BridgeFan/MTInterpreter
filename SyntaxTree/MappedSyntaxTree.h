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
	bool mapTree(const SyntaxTree& tree);
};

#endif //MTINTERPRETER_MAPPEDSYNTAXTREE_H
