#pragma once
#include "Token.h"

class IdToken: public Token {
	std::string value;
public:
	IdToken(int line, int column, const std::string &raw);
	IdToken()=default;
	IdToken(const IdToken&)=default;
	const std::string& getValue() const;
};
