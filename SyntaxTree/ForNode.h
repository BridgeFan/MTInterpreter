//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_FORNODE_H
#define MTINTERPRETER_FORNODE_H
#include "Block.h"
#include "AssignNode.h"
#include "InitNode.h"
#include <variant>

struct ForNode: public Line {
	InitNode init;
	Expression condition;
	AssignNode assignNode;
	Block stat;
};

#endif //MTINTERPRETER_FORNODE_H
