//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_FUNCALL_H
#define MTINTERPRETER_FUNCALL_H

#include "Line.h"
#include <string>
#include <vector>
#include <memory>

class Expression;

struct FunCall: public Line {
	std::string name;
	std::vector<std::unique_ptr<Expression> > params;
	virtual void accept(Visitor& visitor) override;
};

#endif //MTINTERPRETER_FUNCALL_H
