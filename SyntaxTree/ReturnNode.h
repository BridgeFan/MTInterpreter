//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_RETURNNODE_H
#define MTINTERPRETER_RETURNNODE_H
#include "Expression.h"
#include <optional>
#include "Line.h"

struct ReturnNode: public Line {
	ReturnNode(): returnedValue(std::nullopt) {}
	ReturnNode(Expression expr): returnedValue(expr) {}
	std::optional<Expression> returnedValue;
};

#endif //MTINTERPRETER_RETURNNODE_H
