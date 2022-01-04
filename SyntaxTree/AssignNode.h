//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_ASSIGNNODE_H
#define MTINTERPRETER_ASSIGNNODE_H

#include "Expression.h"
#include "Line.h"
enum AssignNodeType: uint16_t {
	assign2,
	addAssign2,
	minusAssign2,
	multAssign2,
	divAssign2,
	moduloAssign2,
};

struct AssignNode: public Line {
	std::string id;
	AssignNodeType type;
	std::unique_ptr<Expression> expression;
	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	}
};

#endif //MTINTERPRETER_ASSIGNNODE_H
