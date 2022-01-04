//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_EXPRESSION_H
#define MTINTERPRETER_EXPRESSION_H


#include <optional>
#include <functional>
#include <variant>
#include "../Token/Token.h"
#include "SyntaxTreeElement.h"

enum OperatorType: uint16_t {
	none,
	negation,
	minus,
	add,
	mult,
	divi,
	mod,
	eq,
	neq,
	meq,
	mor,
	les,
	leq,
	Or,
	And,
	toIntConversion,
	toDoubleConversion,
};

struct FunCall;
struct Expression: SyntaxTreeElement {
	std::unique_ptr<Expression> expression1;
	std::unique_ptr<Expression> expression2; //can be null
	OperatorType  op; //can be null for simple expressions (derived types)
	virtual ~Expression()=default;
	virtual void accept(Visitor& visitor) override {
		visitor.visit(*this);
	}
};

struct IdExpression: public Expression {
	std::string value;
	virtual void accept(Visitor& visitor) override {
		visitor.visit(*this);
	}
};

struct StringExpression: public Expression {
	std::string value;
	virtual void accept(Visitor& visitor) override {
		visitor.visit(*this);
	}
};

struct NumberExpression: public Expression {
	std::variant<int64_t, double> value;
	virtual void accept(Visitor& visitor) override {
		visitor.visit(*this);
	}
};

struct FunCallExpression: public Expression {
	std::unique_ptr<FunCall> token;
	virtual void accept(Visitor& visitor) override {
		if(token)
			visitor.visit(*token);
	}
};


#endif //MTINTERPRETER_EXPRESSION_H
