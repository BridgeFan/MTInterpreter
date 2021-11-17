//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_ASSIGN_H
#define MTINTERPRETER_ASSIGN_H

#include "Token.h"

enum AssignType: uint16_t {
	assign,
	addAssign,
	minusAssign,
	multAssign,
	divAssign,
	moduloAssign,
};

class Assign: public Token {
	AssignType subtype;
public:
	Assign(int line, int column, AssignType subtype);
	bool addChar(char c) override;
	AssignType getSubtype() const;
};


#endif //MTINTERPRETER_ASSIGN_H
