//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_PARAMETER_H
#define MTINTERPRETER_PARAMETER_H

#include <memory>
#include "../Token/TypeName.h"
#include "../Token/IdToken.h"
#include "Line.h"

struct Parameter: public Line {
	TypeName type;
	IdToken name;
	Parameter(const TypeName& type, const IdToken& name): type(type), name(name) {}
};


#endif //MTINTERPRETER_PARAMETER_H
