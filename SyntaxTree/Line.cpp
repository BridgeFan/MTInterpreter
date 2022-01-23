//
// Created by kamil-hp on 23.01.2022.
//
#include "Line.h"
#include "../Interpreter/Visitor.h"

void Line::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void LoopModLine::accept(Visitor& visitor) {
	visitor.visit(*this);
}