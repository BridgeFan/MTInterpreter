#pragma once
#include "Token.h"

class IdToken:public Token {
public:
	IdToken(int line, int column, const std::string &raw);

	const std::string& getValue() const {return raw;}
	bool addChar(char c) override;
};
