//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_MULTOP_H
#define MTINTERPRETER_MULTOP_H

#include <cstdint>
#include "Token.h"

enum MultOpType: uint16_t {
	multiply,
	divide,
	modulo
};

class MultOp: public Token {
	MultOpType subtype;
public:
	MultOp(int line, int column, MultOpType type);
	MultOpType getSubtype() const;
};


#endif //MTINTERPRETER_MULTOP_H
