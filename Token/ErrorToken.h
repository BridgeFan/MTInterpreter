//
// Created by kamil-hp on 18.11.2021.
//

#ifndef MTINTERPRETER_ERRORTOKEN_H
#define MTINTERPRETER_ERRORTOKEN_H

#include "Token.h"
#include "Assign.h"

enum ErrorType: uint16_t {
	unexpectedCharacter,
	overflow, //too great value or too long name
	unexpectedEof,
	unexpectedToken,
	wrongEnd,
	modifyAssignToUninitialized,
};

class ErrorToken: public Token {
	std::string value;
	TokenType expectedType;
	ErrorType errorType;
	AssignType gotAssignType;
	TokenType gotType;
public:
	ErrorToken(int line, int column, const std::string &raw, TokenType expectedType, ErrorType eType=unexpectedCharacter);
	ErrorToken(int line, int column, TokenType gotType, TokenType expectedType);
	ErrorToken(int line, int column, AssignType gotAssignType);
	TokenType getExpected() const {return expectedType;}
	ErrorType getErrorType() const {return errorType;}
	TokenType getGotType() const {return gotType;}
	const std::string& getValue() const {return value;}
};


#endif //MTINTERPRETER_ERRORTOKEN_H
