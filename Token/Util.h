//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_UTIL_H
#define MTINTERPRETER_UTIL_H

const std::string defined="();.,!-=+*/%|&{}<>";

namespace Util {
	static constexpr int maxLength = 128; //maximum length of word
	bool isLetter(char a) {
		return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a=='_';
	}

	bool isDigit(char a) {
		return (a >= '0' && a <= '9');
	}

	bool isWhite(char a) {
		return std::isspace(a);
	}

	bool isDefined(char a) {
		return defined.find(a) != std::string::npos;
	}

	bool isStringCompatible(char a) {
		return (a>=32 && a!=127);
	}
}

#endif //MTINTERPRETER_UTIL_H
