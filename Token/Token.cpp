//
// Created by kamil-hp on 17.11.2021.
//
#include "Token.h"
#include <utility>


Token::Token(TokenType type, int line, int column): type(type), line(line), column(column) {}

TokenType Token::getType() const {
	return type;
}

int Token::getColumn() const {
	return column;
}

int Token::getLine() const {
	return line;
}


//TO REMOVE
bool Token::addChar(char c) {
	return true;
}

