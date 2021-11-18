//
// Created by kamil-hp on 17.11.2021.
//

#include "MultOp.h"

bool MultOp::addChar(char c) {
	return true;
}

MultOp::MultOp(int line, int column, MultOpType type): Token(line, column) {
	this->type = MultOp_;
	subtype = type;
}

MultOpType MultOp::getSubtype() const {
	return subtype;
}