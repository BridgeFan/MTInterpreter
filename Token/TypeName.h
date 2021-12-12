//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_TYPENAME_H
#define MTINTERPRETER_TYPENAME_H

#include <cstdint>
#include "Token.h"

enum TypeNameType: uint16_t {
	intType,
	doubleType,
};

class TypeName: public Token {
	TypeNameType subtype;
public:
	TypeName(int line, int column, TypeNameType subtype);
	TypeName()=default;
	TypeNameType getSubtype() const;
};


#endif //MTINTERPRETER_TYPENAME_H
