//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_IFNODE_H
#define MTINTERPRETER_IFNODE_H

#include "Expression.h"
#include "Block.h"
#include <variant>

struct IfNode: public Line {
	std::unique_ptr<Expression> condition;
	Block stat;
	std::optional<Block> elseStat;
};

#endif //MTINTERPRETER_IFNODE_H
