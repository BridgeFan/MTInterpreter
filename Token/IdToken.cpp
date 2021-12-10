//
// Created by kamil-hp on 17.11.2021.
//

#include "IdToken.h"
#include "../ErrorHandler.h"
#include "../DataSource/DataSource.h"
#include "Util.h"

bool IdToken::addChar(char c) {
	if(value.length()>=Util::maxLength)
		throw std::bad_exception();
	if (Util::isLetter(c) || Util::isDigit(c))
		value += c;
	else if (Util::isDefined(c) || Util::isWhite(c) || c==eof)
		return true;
	else
		throw std::exception();
	return false;
}

IdToken::IdToken(int line, int column, const std::string &raw) : Token(Id_, line, column) {
	value=raw;
}
