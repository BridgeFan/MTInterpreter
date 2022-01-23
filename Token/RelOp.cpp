//
// Created by kamil-hp on 17.11.2021.
//

#include "RelOp.h"

RelOp::RelOp(int line, int column, RelOpType type): Token(RelOp_, line, column) {
	subtype = type;
}

RelOpType RelOp::getSubtype() const {
	return subtype;
}