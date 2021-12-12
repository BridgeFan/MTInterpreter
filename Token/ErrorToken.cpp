//
// Created by kamil-hp on 18.11.2021.
//

#include "ErrorToken.h"

ErrorToken::ErrorToken(int line, int column, const std::string &raw, TokenType expectedType, ErrorType eType): Token(Error_, line, column), value(raw) {
	errorType = eType;
	this->expectedType=expectedType;
}

ErrorToken::ErrorToken(int line, int column, TokenType gotType, TokenType expectedType): ErrorToken(line,column,"",expectedType,unexpectedToken) {
	this->gotType=gotType;
}
ErrorToken::ErrorToken(int line, int column, AssignType gotAssignType): ErrorToken(line,column,"",Assign_,modifyAssignToUninitialized) {
	this->gotAssignType=gotAssignType;
}