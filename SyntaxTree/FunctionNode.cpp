//
// Created by kamil-hp on 23.01.2022.
//

#include "FunctionNode.h"
#include "../Interpreter/Visitor.h"

void FunctionNode::accept(Visitor& visitor) {
	visitor.visit(*this);
}
