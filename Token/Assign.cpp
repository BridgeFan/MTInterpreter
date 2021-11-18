//
// Created by kamil-hp on 17.11.2021.
//

#include "Assign.h"

bool Assign::addChar(char c) {
	return true;
}

Assign::Assign(int line, int column, AssignType subtype): Token(line, column) {
	type = TokenType::Assign_;
	this->subtype = subtype;
}

AssignType Assign::getSubtype() const {
	return subtype;
}