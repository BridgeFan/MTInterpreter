//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_PARAMETER_H
#define MTINTERPRETER_PARAMETER_H

#include <memory>
#include <utility>
#include <string>
#include "Line.h"

enum TypeType: uint16_t;

struct Parameter: public Line {
	TypeType type;
	std::string name;
	Parameter(const TypeType& type, std::string name): type(type), name(std::move(name)) {}
	virtual void accept(Visitor& visitor) override;
};


#endif //MTINTERPRETER_PARAMETER_H
