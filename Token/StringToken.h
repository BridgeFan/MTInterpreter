//
// Created by kamil-hp on 18.11.2021.
//

#ifndef MTINTERPRETER_STRINGTOKEN_H
#define MTINTERPRETER_STRINGTOKEN_H

#include "Token.h"

class StringToken: public Token {
	std::string value;
public:
	StringToken(int line, int column, const std::string &raw);
	const std::string& getValue() const {return value;}
};


#endif //MTINTERPRETER_STRINGTOKEN_H
