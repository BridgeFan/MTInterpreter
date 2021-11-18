//
// Created by kamil-hp on 18.11.2021.
//

#include "StringToken.h"
#include "../ErrorHandler.h"

bool StringToken::addChar(char c) {
	if(value.length()>=Token::maxLength)
		throw std::bad_exception();
	if(isSpecialEnter) {
		switch(c) {
			case '\\':
				value+='\\';
				return false;
			case '\'':
				value+='\'';
				return false;
			case '\"':
				value+='\"';
				return false;
			case '\?':
				value+='\?';
				return false;
			case 'a':
				value+='\a';
				return false;
			case 'b':
				value+='\b';
				return false;
			case 'f':
				value+='\f';
				return false;
			case 'n':
				value+='\n';
				return false;
			case 'r':
				value+='\r';
				return false;
			case 't':
				value+='\t';
				return false;
			case 'v':
				value+='\v';
				return false;
			default:
				value+=c;
				throw std::exception();

		}
	}
	if (c=='"')
		return true;
	if(c=='\\') {
		isSpecialEnter=true;
		return false;
	}
	if (isStringCompatible(c)) {
		value += c;
		return false;
	}
	else {
		//error
		value += c;
		throw std::exception();
	}
	return false;
}

StringToken::StringToken(int line, int column, const std::string &raw) : Token(line, column) {
	value = raw;
	type = TokenType::String_;
}