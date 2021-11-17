//
// Created by kamil-hp on 17.11.2021.
//

#include "LogicOp.h"
bool LogicOp::addChar(char c) {
	return true;
}

LogicOp::LogicOp(int line, int column, LogicOpType type): Token(line, column, "") {
	this->type = Logic_;
	subtype = type;
}

LogicOpType LogicOp::getSubtype() const {
	return subtype;
}