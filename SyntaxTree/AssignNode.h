//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_ASSIGNNODE_H
#define MTINTERPRETER_ASSIGNNODE_H

#include "../Token/Assign.h"
#include "Expression.h"
#include "Line.h"

struct AssignNode: public Line {
	std::unique_ptr<IdToken> id;
	std::unique_ptr<Assign> assign;
	Expression expression;
};

#endif //MTINTERPRETER_ASSIGN_H
