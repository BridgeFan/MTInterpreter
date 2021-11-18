//
// Created by kamil-hp on 17.11.2021.
//

#include "IdToken.h"
#include "../ErrorHandler.h"

bool IdToken::addChar(char c) {
	if(value.length()>=Token::maxLength)
		throw std::bad_exception();
	if (isLetter(c) || isDigit(c))
		value += c;
	else if (isDefined(c) || isWhite(c))
		return true;
	else
		throw std::exception();
	return false;
}

IdToken::IdToken(int line, int column, const std::string &raw) : Token(line, column) {
	value=raw;
	type = TokenType::Id_;
}
