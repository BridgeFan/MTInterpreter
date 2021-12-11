//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_INITNODE_H
#define MTINTERPRETER_INITNODE_H
#include <memory>
#include "../Token/IdToken.h"
#include "../Token/TypeName.h"
#include "Line.h"

struct InitNode: public Line {
	std::unique_ptr<TypeName> type;
	std::vector<IdToken> name;
};

#endif //MTINTERPRETER_INITNODE_H
