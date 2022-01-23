//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_CONVERSION_H
#define MTINTERPRETER_CONVERSION_H

#include "Token.h"

enum ConversionType: uint16_t {
	toInt,
	toDouble,
};

class Conversion: public Token {
	ConversionType subtype;
public:
	Conversion(int line, int column, ConversionType type);
	ConversionType getSubtype() const;
};


#endif //MTINTERPRETER_CONVERSION_H
