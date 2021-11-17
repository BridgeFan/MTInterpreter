//
// Created by kamil-hp on 17.11.2021.
//

#include "IdToken.h"

void IdToken::addChar(char c) {
	raw+=c;
}

IdToken::IdToken(int line, int column, const std::string &raw) : Token(line, column, raw) {
	type = TokenType::Id;
}
