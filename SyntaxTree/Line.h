//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_LINE_H
#define MTINTERPRETER_LINE_H

#include <cstdint>
#include "SyntaxTreeElement.h"

enum LoopModT: uint16_t {
	continueType,
	breakType
};

class Line: public SyntaxTreeElement {
public:
	virtual ~Line() = default;
	virtual void accept(Visitor& visitor) override;
};

class LoopModLine: public Line {
public:
	LoopModT type;
	void accept(Visitor& visitor) override;
	explicit LoopModLine(LoopModT type): type(type) {}
};

#endif //MTINTERPRETER_LINE_H
