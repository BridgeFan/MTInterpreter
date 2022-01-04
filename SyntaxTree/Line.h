//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_LINE_H
#define MTINTERPRETER_LINE_H

#include "SyntaxTreeElement.h"
#include "../Interpreter/Visitor.h"

class Line: public SyntaxTreeElement {
public:
	virtual ~Line() = default;
	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	}
};

#endif //MTINTERPRETER_LINE_H
