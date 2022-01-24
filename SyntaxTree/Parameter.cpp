//
// Created by kamil-hp on 23.01.2022.
//
#include "Parameter.h"
#include "../Interpreter/Visitor.h"

void Parameter::accept(Visitor& visitor) {
	visitor.visit(*this);
}