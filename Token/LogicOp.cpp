//
// Created by kamil-hp on 17.11.2021.
//

#include "LogicOp.h"

LogicOp::LogicOp(int line, int column, LogicOpType type): Token(Logic_, line, column) {
	subtype = type;
}

LogicOpType LogicOp::getSubtype() const {
	return subtype;
}