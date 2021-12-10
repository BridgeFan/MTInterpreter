//
// Created by kamil-hp on 17.11.2021.
//

#include "Conversion.h"

Conversion::Conversion(int line, int column, ConversionType type): Token(Conversion_, line, column) {
	subtype = type;
}

ConversionType Conversion::getSubtype() const {
	return subtype;
}