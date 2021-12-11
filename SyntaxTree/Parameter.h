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
	std::unique_ptr<TypeName> type;
	std::unique_ptr<IdToken> name;
	Parameter(std::unique_ptr<TypeName>&& t, std::unique_ptr<IdToken>&& n): type(std::move(t)), name(std::move(n)) {}
};


#endif //MTINTERPRETER_PARAMETER_H
