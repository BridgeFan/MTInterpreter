//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_LINE_H
#define MTINTERPRETER_LINE_H

#include "SyntaxTreeElement.h"
#include "../Interpreter/Visitor.h"

enum LoopModT: uint16_t {
	continueType,
	breakType
};

class Line: public SyntaxTreeElement {
public:
	virtual ~Line() = default;
	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	}
};

class LoopModLine: public Line {
public:
	LoopModT type;
	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	}
	explicit LoopModLine(LoopModT type): type(type) {}
};

#endif //MTINTERPRETER_LINE_H
