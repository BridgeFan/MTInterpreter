//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_FUNCTIONNODE_H
#define MTINTERPRETER_FUNCTIONNODE_H

#include <memory>
#include <string>
#include <utility>
#include "SyntaxTreeElement.h"
#include "Block.h"
#include "Parameter.h"

enum TypeType: uint16_t;

struct FunctionNode: SyntaxTreeElement {
	std::string name;
	TypeType returnedType;
	std::vector<Parameter> parameters;
	Block block;
	virtual void accept(Visitor& visitor) override;
	explicit FunctionNode(std::string name="", TypeType returned=(TypeType)2u, std::vector<Parameter> parameters={}):
		name(std::move(name)), returnedType(returned), parameters(std::move(parameters)) {}
};


#endif //MTINTERPRETER_FUNCTIONNODE_H
