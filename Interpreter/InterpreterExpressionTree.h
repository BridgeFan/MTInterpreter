//
// Created by kamil-hp on 04.01.2022.
//

#ifndef MTINTERPRETER_INTERPRETEREXPRESSIONTREE_H
#define MTINTERPRETER_INTERPRETEREXPRESSIONTREE_H
#include <memory>
#include <cstdint>
#include <variant>
#include <string>
#include <cmath>
#include "../SyntaxTree/Expression.h"
#include "../SyntaxTree/InitNode.h"
#include "Visitor.h"


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
enum TypeType: uint16_t;

InterpreterValue getBinaryOperationValue(const InterpreterValue& value1, const InterpreterValue& value2, OperatorType type);
InterpreterValue getUnaryOperationValue(const InterpreterValue& value, OperatorType type);


#endif //MTINTERPRETER_INTERPRETEREXPRESSIONTREE_H
