//
// Created by kamil-hp on 17.11.2021.
//

#include "IdToken.h"
#include "../ErrorHandler.h"

bool IdToken::addChar(char c) {
	if (isLetter(c) || isDigit(c))
		raw += c;
	else if (isDefined(c) || isWhite(c))
		return true;
	else {
		//error
		ErrorHandler::addError(ErrorType::ScanerError,line, column, raw, " unexpected character in literal");
	}
	return false;
}

IdToken::IdToken(int line, int column, const std::string &raw) : Token(line, column, raw) {
	type = TokenType::Id_;
}
