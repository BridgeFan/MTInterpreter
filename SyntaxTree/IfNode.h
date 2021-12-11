//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_IFNODE_H
#define MTINTERPRETER_IFNODE_H

#include "Expression.h"
#include "Block.h"
#include <variant>

struct IfNode: public Line {
	Expression condition;
	Block stat;
	std::variant<Block, std::nullptr_t> elseStat;
};

#endif //MTINTERPRETER_IFNODE_H
