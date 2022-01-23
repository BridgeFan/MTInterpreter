//
// Created by kamil-hp on 23.01.2022.
//
#include "FunCall.h"
#include "../Interpreter/Visitor.h"

void FunCall::accept(Visitor& visitor) {
	visitor.visit(*this);
}
