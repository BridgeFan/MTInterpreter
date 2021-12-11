//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_FUNCALL_H
#define MTINTERPRETER_FUNCALL_H
#include "../Token/IdToken.h"
#include "Expression.h"
#include "Line.h"

struct FunCall: public Line {
	std::unique_ptr<IdToken> name;
	std::vector<std::reference_wrapper<Expression> > params;
};

#endif //MTINTERPRETER_FUNCALL_H
