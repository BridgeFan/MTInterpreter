//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_EXPRESSION_H
#define MTINTERPRETER_EXPRESSION_H


#include <optional>
#include <functional>
#include "../Token/Token.h"
#include "../Token/IdToken.h"
#include "../Token/StringToken.h"
#include "../Token/Number.h"

struct FunCall;

struct Expression {
	//TODO: expression
	std::reference_wrapper<Expression> expression1;
	std::optional<std::reference_wrapper<Expression> > expression2; //can be null
	std::optional<std::reference_wrapper<Token> >  op; //can be null for simple expressions (derived types)
};

struct IdExpression: public Expression {
	IdToken token;
};

struct StringExpression: public Expression {
	StringToken token;
};

struct NumberExpression: public Expression {
	Number token;
};

struct FunCallExpression: public Expression {
	std::reference_wrapper<FunCall> token;
};


#endif //MTINTERPRETER_EXPRESSION_H
