//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_INITNODE_H
#define MTINTERPRETER_INITNODE_H
#include <memory>
#include <utility>
#include "Line.h"
#include "Expression.h"
#include <vector>

enum TypeType: uint16_t {
	int_,
	double_,
	void_, //semantic analyzer will use this value as error type (of f.e. unknown function or variable)
	string_ //for semantic analyzer
};

struct InitNode: public Line {
	TypeType type;
	std::vector<std::pair<std::string, std::shared_ptr<Expression>>> vars;
	InitNode(TypeType type, std::vector<std::pair<std::string, std::shared_ptr<Expression>>> vars): type(type), vars(std::move(vars)) {}
	InitNode()=default;
	InitNode(const InitNode&)=default;
	void accept(Visitor& visitor) override {
		visitor.visit(*this);
	}
};

#endif //MTINTERPRETER_INITNODE_H
