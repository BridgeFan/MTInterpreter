//
// Created by kamil-hp on 17.11.2021.
//

#include "IdToken.h"

IdToken::IdToken(int line, int column, const std::string &raw) : Token(Id_, line, column) {
	value=raw;
}

const std::string &IdToken::getValue() const {
	return value;
}
