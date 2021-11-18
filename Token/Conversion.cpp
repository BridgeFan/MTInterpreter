//
// Created by kamil-hp on 17.11.2021.
//

#include "Conversion.h"

bool Conversion::addChar(char c) {
	return true;
}

Conversion::Conversion(int line, int column, ConversionType type): Token(line, column) {
	this->type = Conversion_;
	subtype = type;
}

ConversionType Conversion::getSubtype() const {
	return subtype;
}