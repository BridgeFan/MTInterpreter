//
// Created by kamil-hp on 17.11.2021.
//

#include "LoopMod.h"

LoopMod::LoopMod(int line, int column, LoopModType subtype): Token(LoopMod_, line, column) {
	this->subtype = subtype;
}

LoopModType LoopMod::getSubtype() const {
	return subtype;
}
