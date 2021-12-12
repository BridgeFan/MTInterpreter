//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_FORNODE_H
#define MTINTERPRETER_FORNODE_H
#include "Block.h"
#include "AssignNode.h"
#include "InitNode.h"
#include <variant>
#include <memory>

struct ForNode: public Line {
	InitNode init;
	std::unique_ptr<Expression> condition;
	std::optional<AssignNode> assignNodePre, assignNodeEach;
	Block stat;
	ForNode()=default;
};

#endif //MTINTERPRETER_FORNODE_H
