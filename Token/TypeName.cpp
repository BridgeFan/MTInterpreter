//
// Created by kamil-hp on 17.11.2021.
//

#include "TypeName.h"

bool TypeName::addChar(char c) {
	return true;
}

TypeName::TypeName(int line, int column, TypeNameType subtype): Token(TypeName_, line, column) {
	this->subtype = subtype;
}

TypeNameType TypeName::getSubtype() const {
	return subtype;
}