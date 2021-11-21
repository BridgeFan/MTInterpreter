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
				break;
			case '\'':
				value+='\'';
				break;
			case '\"':
				value+='\"';
				break;
			case '\?':
				value+='\?';
				break;
			case 'a':
				value+='\a';
				break;
			case 'b':
				value+='\b';
				break;
			case 'f':
				value+='\f';
				break;
			case 'n':
				value+='\n';
				break;
			case 'r':
				value+='\r';
				break;
			case 't':
				value+='\t';
				break;
			case 'v':
				value+='\v';
				break;
			default:
				value+=c;
				throw std::exception();
		}
		isSpecialEnter=false;
		return false;
	}
	if (c=='\"')
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