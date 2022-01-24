//
// Created by kamil-hp on 23.01.2022.
//

#include "Block.h"
#include "../Interpreter/Visitor.h"

void Block::accept(Visitor& visitor) {
	visitor.visit(*this);
}
