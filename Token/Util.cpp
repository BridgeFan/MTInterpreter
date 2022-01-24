//
// Created by kamil-hp on 10.12.2021.
//
#include <string>
#include "Util.h"

const std::string defined="();.,!-=+*/%|&{}<>";

bool Util::isLetter(char a) {
	return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a=='_';
}

bool Util::isDigit(char a) {
	return (a >= '0' && a <= '9');
}

bool Util::isWhite(char a) {
	return std::isspace(a);
}

bool Util::isDefined(char a) {
	return defined.find(a) != std::string::npos;
}

bool Util::isStringCompatible(char a) {
	return (a>=32 && a!=127 && a!='\"');
}
