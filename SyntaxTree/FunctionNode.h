//
// Created by kamil-hp on 10.12.2021.
//

#ifndef MTINTERPRETER_FUNCTIONNODE_H
#define MTINTERPRETER_FUNCTIONNODE_H

#include <memory>
#include "../Token/IdToken.h"

struct FunctionNode {
	std::unique_ptr<IdToken> id; //only TypeName or Void_
	std::unique_ptr<Token> returnedType;
	std::vector<Parameter> parameters;
	Block block;
};


#endif //MTINTERPRETER_FUNCTIONNODE_H
