//
// Created by kamil-hp on 18.11.2021.
//

#ifndef MTINTERPRETER_NUMBER_H
#define MTINTERPRETER_NUMBER_H

#include <variant>
#include "Token.h"

enum NumberState: uint16_t {
	plus,
	plus_divide,
};

class Number: public Token {
	NumberState state;
	int64_t value1;
	int64_t value2=0;
	int value2size=0;
public:
	Number(int line, int column, int64_t val, NumberState state);

	std::variant<int64_t, double> getValue() const;
	bool addChar(char c) override;
};


#endif //MTINTERPRETER_NUMBER_H
