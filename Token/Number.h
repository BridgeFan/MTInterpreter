//
// Created by kamil-hp on 18.11.2021.
//

#ifndef MTINTERPRETER_NUMBER_H
#define MTINTERPRETER_NUMBER_H

#include <variant>
#include "Token.h"

class Number: public Token {
	std::variant<int64_t, double> value;
public:
	Number(int line, int column, int64_t val);
	Number(int line, int column, double val);
	const std::variant<int64_t, double>& getValue() const {return value;}
};


#endif //MTINTERPRETER_NUMBER_H
