//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_EXPRESSION_H
#define MTINTERPRETER_EXPRESSION_H


#include <optional>
#include <functional>
#include <variant>
#include <memory>
#include "../Token/Token.h"
#include "SyntaxTreeElement.h"
#include "EvaluableSyntaxTreeElement.h"
#include "../Interpreter/Visitor.h"
#include "FunCall.h"

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

struct Expression: public EvaluableSyntaxTreeElement {
	std::unique_ptr<Expression> expression1;
	std::unique_ptr<Expression> expression2; //can be null
	OperatorType  op; //can be null for simple expressions (derived types)
	virtual ~Expression()=default;
	virtual InterpreterValue calculate(Visitor& visitor) override;
};

struct IdExpression: public Expression {
	std::string value;
	virtual InterpreterValue calculate(Visitor& visitor) override;
};

struct StringExpression: public Expression {
	std::string value;
	virtual InterpreterValue calculate(Visitor& visitor) override;
};

struct NumberExpression: public Expression {
	std::variant<int64_t, double> value;
	virtual InterpreterValue calculate(Visitor& visitor) override;
};

struct FunCallExpression: public Expression {
	std::unique_ptr<FunCall> funCall;
	virtual InterpreterValue calculate(Visitor& visitor) override;
};


#endif //MTINTERPRETER_EXPRESSION_H
