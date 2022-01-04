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


enum TypeType: uint16_t;
typedef std::variant<int64_t, double, std::nullptr_t, std::string> InterpreterValue;

InterpreterValue getBinaryOperationValue(const InterpreterValue& value1, const InterpreterValue& value2, OperatorType type);
InterpreterValue getUnaryOperationValue(const InterpreterValue& value, OperatorType type);

class InterpreterExpressionTree {
private:
	std::shared_ptr<InterpreterExpressionTree> left, right, parent;
public:
	InterpreterValue value=nullptr;
	InterpreterExpressionTree()=default;
	InterpreterExpressionTree(InterpreterValue value, InterpreterExpressionTree* parent): value(std::move(value)), parent(parent) {}
	void setLeft(TypeType lType) {left=std::make_unique<InterpreterExpressionTree>(lType,this);}
	void unsetLeft() {left.reset();}
	[[nodiscard]] InterpreterExpressionTree* getLeft() const {return left.get();}
	void setRight(TypeType rType) {left=std::make_unique<InterpreterExpressionTree>(rType,this);}
	void unsetRight() {right.reset();}
	[[nodiscard]] InterpreterExpressionTree* getRight() const {return right.get();}
	[[nodiscard]] InterpreterExpressionTree* getParent() const {return parent.get();}
};

#endif //MTINTERPRETER_INTERPRETEREXPRESSIONTREE_H
