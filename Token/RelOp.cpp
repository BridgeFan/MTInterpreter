//
// Created by kamil-hp on 17.11.2021.
//

#include "RelOp.h"
bool RelOp::addChar(char c) {
	return true;
}

RelOp::RelOp(int line, int column, RelOpType type): Token(line, column, "") {
	this->type = RelOp_;
	subtype = type;
}

RelOpType RelOp::getSubtype() const {
	return subtype;
}