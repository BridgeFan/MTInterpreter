//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_FUNCALL_H
#define MTINTERPRETER_FUNCALL_H
#include "Expression.h"
#include "Line.h"

struct FunCall: public Line {
	std::string name;
	std::vector<std::unique_ptr<Expression> > params;
};

#endif //MTINTERPRETER_FUNCALL_H
