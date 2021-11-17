//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_RELOP_H
#define MTINTERPRETER_RELOP_H

#include "Token.h"

enum RelOpType: uint16_t {
	equal,
	not_equal,
	more_equal,
	more,
	less,
	less_equal
};

class RelOp: public Token {
	RelOpType subtype;
public:
	RelOp(int line, int column, RelOpType type);
	bool addChar(char c) override;
	RelOpType getSubtype() const;
};


#endif //MTINTERPRETER_RELOP_H
