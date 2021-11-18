//
// Created by kamil-hp on 17.11.2021.
//

#include "LoopMod.h"

bool LoopMod::addChar(char c) {
	return true;
}

LoopMod::LoopMod(int line, int column, LoopModType subtype): Token(line, column) {
	type = TokenType::LoopMod_;
	this->subtype = subtype;
}

LoopModType LoopMod::getSubtype() const {
	return subtype;
}
