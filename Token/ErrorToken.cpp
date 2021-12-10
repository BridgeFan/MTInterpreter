//
// Created by kamil-hp on 18.11.2021.
//

#include "ErrorToken.h"

bool ErrorToken::addChar(char c) {
	return true;
}

ErrorToken::ErrorToken(int line, int column, const std::string &raw, TokenType expectedType, ErrorType eType): Token(Error_, line, column), value(raw) {
	errorType = eType;
	this->expectedType=expectedType;
}
