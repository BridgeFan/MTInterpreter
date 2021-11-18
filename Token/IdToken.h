#pragma once
#include "Token.h"

class IdToken:public Token {
	std::string value;
public:
	IdToken(int line, int column, const std::string &raw);

	const std::string& getValue() const {return value;}
	bool addChar(char c) override;
};
