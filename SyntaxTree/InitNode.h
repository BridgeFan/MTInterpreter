//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_INITNODE_H
#define MTINTERPRETER_INITNODE_H
#include <memory>
#include <utility>
#include "../Token/IdToken.h"
#include "../Token/TypeName.h"
#include "Line.h"

struct InitNode: public Line {
	TypeName type;
	std::vector<std::pair<IdToken, std::optional<std::reference_wrapper<Expression>>>> vars;
	InitNode(TypeName type, std::vector<std::pair<IdToken, std::optional<std::reference_wrapper<Expression>>>> vars): type(std::move(type)), vars(std::move(vars)) {}
	InitNode()=default;
	InitNode(const InitNode&)=default;
};

#endif //MTINTERPRETER_INITNODE_H
