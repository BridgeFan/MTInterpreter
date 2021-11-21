//
// Created by kamil-hp on 18.11.2021.
//

#ifndef MTINTERPRETER_ERRORTOKEN_H
#define MTINTERPRETER_ERRORTOKEN_H

#include "Token.h"

enum ErrorType: uint16_t {
	unexpectedCharacter,
	overflow, //too great value or too long name
};

class ErrorToken: public Token {
	std::string value;
	TokenType expectedType;
	ErrorType errorType;
public:
	ErrorToken(int line, int column, const std::string &raw, TokenType expectedType, ErrorType eType=unexpectedCharacter);
	TokenType getExpected() const {return expectedType;}
	ErrorType getErrorType() const {return errorType;}
	const std::string& getValue() const {return value;}
	bool addChar(char c) override;
};


#endif //MTINTERPRETER_ERRORTOKEN_H
