//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_LOOPMOD_H
#define MTINTERPRETER_LOOPMOD_H
#include "Token.h"

enum LoopModType: uint16_t {
	continueMod,
	breakMod,
};

class LoopMod: public Token {
	LoopModType subtype;
public:
	LoopMod(int line, int column, LoopModType subtype);
	LoopModType getSubtype() const;
};


#endif //MTINTERPRETER_LOOPMOD_H
