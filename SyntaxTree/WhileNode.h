//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_WHILENODE_H
#define MTINTERPRETER_WHILENODE_H

#include "Block.h"
#include "Expression.h"

struct WhileNode: public Line {
	std::unique_ptr<Expression> condition;
	Block stat;
	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	}
};

#endif //MTINTERPRETER_WHILENODE_H
