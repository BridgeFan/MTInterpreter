//
// Created by kamil-hp on 17.11.2021.
//
#include "Token.h"
#include <utility>

Token::Token(int line, int column, std::string raw): line(line), column(column), raw(std::move(raw)) {}

TokenType Token::getType() const {
	return type;
}

void Token::finish() {

}
