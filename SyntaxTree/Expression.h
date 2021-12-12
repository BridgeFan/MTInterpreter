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
	std::optional<std::unique_ptr<Expression> > expression1;
	std::optional<std::unique_ptr<Expression> > expression2; //can be null
	std::optional<std::unique_ptr<Token> >  op; //can be null for simple expressions (derived types)
	virtual ~Expression()=default;
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
	std::optional<std::unique_ptr<FunCall> > token;
};


#endif //MTINTERPRETER_EXPRESSION_H
