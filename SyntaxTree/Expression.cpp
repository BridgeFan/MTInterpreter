//
// Created by kamil-hp on 23.01.2022.
//
#include "Expression.h"
#include "../Interpreter/Visitor.h"

InterpreterValue Expression::calculate(Visitor& visitor) {
	return visitor.calculate(*this);
}

InterpreterValue IdExpression::calculate(Visitor& visitor) {
	return visitor.calculate(*this);
}

InterpreterValue StringExpression::calculate(Visitor& visitor) {
	return visitor.calculate(*this);
}

InterpreterValue FunCallExpression::calculate(Visitor& visitor) {
	return visitor.calculate(*this);
}

InterpreterValue NumberExpression::calculate(Visitor& visitor) {
	return visitor.calculate(*this);
}