//
// Created by kamil-hp on 17.11.2021.
//

#include "MultOp.h"

MultOp::MultOp(int line, int column, MultOpType type): Token(MultOp_, line, column) {
	subtype = type;
}

MultOpType MultOp::getSubtype() const {
	return subtype;
}