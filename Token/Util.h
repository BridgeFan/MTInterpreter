//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_UTIL_H
#define MTINTERPRETER_UTIL_H

namespace Util {
	static constexpr int maxLength = 128; //maximum length of word
	bool isLetter(char a);
	bool isDigit(char a);
	bool isWhite(char a);
	bool isDefined(char a);
	bool isStringCompatible(char a);
}

#endif //MTINTERPRETER_UTIL_H
