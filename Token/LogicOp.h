//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_LOGICOP_H
#define MTINTERPRETER_LOGICOP_H

#include "Token.h"

enum LogicOpType: uint16_t {
	or_,
	and_
};

class LogicOp: public Token {
	LogicOpType subtype;
public:
	LogicOp(int line, int column, LogicOpType type);
	bool addChar(char c) override;
	LogicOpType getSubtype() const;
};


#endif //MTINTERPRETER_LOGICOP_H
