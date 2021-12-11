//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_WHILENODE_H
#define MTINTERPRETER_WHILENODE_H

#include "Block.h"
#include "Expression.h"

struct WhileNode: public Line {
	Expression condition;
	Block stat;
};

#endif //MTINTERPRETER_WHILENODE_H
