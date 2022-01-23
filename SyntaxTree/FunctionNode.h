//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_FUNCTIONNODE_H
#define MTINTERPRETER_FUNCTIONNODE_H

#include <memory>
#include "../Token/IdToken.h"

struct FunctionNode {
	IdToken id;
	std::variant<TypeName, Token> returnedType; //only TypeName or Void_
	std::vector<Parameter> parameters;
	Block block;
};


#endif //MTINTERPRETER_FUNCTIONNODE_H
