//
// Created by kamil-hp on 18.11.2021.
//

#include "StringToken.h"

StringToken::StringToken(int line, int column, const std::string &raw) : Token(String_, line, column) {
	value = raw;
}