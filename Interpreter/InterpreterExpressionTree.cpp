//
// Created by kamil-hp on 04.01.2022.
//
#include "InterpreterExpressionTree.h"
#include <iostream>
#include <stdexcept>
#include <variant>

typedef std::function<InterpreterValue(int64_t,int64_t)> IntBinOp;
typedef std::function<InterpreterValue(double,double)> DoubleBinOp;
typedef std::function<InterpreterValue(const std::string&,const std::string&)> StringBinOp;
typedef std::function<InterpreterValue(int64_t)> IntUnOp;
typedef std::function<InterpreterValue(double)> DoubleUnOp;

class BinaryOperation {
	std::optional<IntBinOp> intFunction;
	std::optional<DoubleBinOp> doubleFunction;
	std::optional<StringBinOp> stringFunction;
	std::string oper;
public:
	explicit BinaryOperation(const std::string& op, IntBinOp&& intFunc): oper(op), intFunction(intFunc) {}
	BinaryOperation(const std::string& op, IntBinOp&& intFunc, DoubleBinOp&& doubleFunc): oper(op), intFunction(intFunc), doubleFunction(doubleFunc) {}
	BinaryOperation(const std::string& op, IntBinOp&& intFunc, DoubleBinOp&& doubleFunc, StringBinOp&& strFunc): oper(op), intFunction(intFunc), doubleFunction(doubleFunc), stringFunction(strFunc) {}
	InterpreterValue operator()(const int64_t &lhs, const int64_t &rhs) {
		if(intFunction)
			return (*intFunction)(lhs, rhs);
		throw std::runtime_error("Illegal int binary operation "+oper);
	}
	InterpreterValue operator()(const double &lhs, const double &rhs) {
		if(doubleFunction)
			return (*doubleFunction)(lhs, rhs);
		throw std::runtime_error("Illegal double binary operation "+oper);
	}
	InterpreterValue operator()(const std::string &lhs, const std::string &rhs) {
		if(stringFunction)
			return (*stringFunction)(lhs, rhs);
		throw std::runtime_error("Illegal string binary operation "+oper);
	}
	InterpreterValue operator()(const double &lhs, const int64_t &rhs) {
		return this->operator()(lhs, (double)rhs);
	}
	InterpreterValue operator()(const int64_t &lhs, const double &rhs) {
		return this->operator()((double)lhs, rhs);
	}
	InterpreterValue operator()(const std::string &lhs, const int64_t &rhs) {
		return this->operator()(lhs,std::to_string(rhs));
	}
	InterpreterValue operator()(const std::string &lhs, const double &rhs) {
		return this->operator()(lhs,std::to_string(rhs));
	}
	InterpreterValue operator()(const int64_t &lhs, const std::string &rhs) {
		return this->operator()(std::to_string(lhs),rhs);
	}
	InterpreterValue operator()(const double &lhs, const std::string &rhs) {
		return this->operator()(std::to_string(lhs),rhs);
	}
	template<typename U, typename V>
	InterpreterValue operator()(const U& lhs, const V &rhs) {throw std::runtime_error("Unknown binary operation "+oper+"parameter types");}
};

class UnaryOperation {
	std::optional<IntUnOp> intFunction;
	std::optional<DoubleUnOp> doubleFunction;
	std::string oper;
public:
	explicit UnaryOperation(const std::string& op, IntUnOp&& intFunc): oper(op), intFunction(intFunc) {}
	UnaryOperation(const std::string& op, IntUnOp&& intFunc, DoubleUnOp&& doubleFunc): oper(op), intFunction(intFunc), doubleFunction(doubleFunc) {}
	InterpreterValue operator()(const int64_t &a) {
		if(intFunction)
			return (*intFunction)(a);
		throw std::runtime_error("Illegal int unary operation "+oper);
	}
	InterpreterValue operator()(const double &a) {
		if(doubleFunction)
			return (*doubleFunction)(a);
		throw std::runtime_error("Illegal double unary operation "+oper);
	}

	template<typename U>
	InterpreterValue operator()(const U& a) {throw std::runtime_error("Unknown unary operation "+oper+" parameter type");}

};

InterpreterValue getBinaryOperationValue(const InterpreterValue& value1, const InterpreterValue& value2, OperatorType type) {
	switch(type) {
		case minus:
			return std::visit(BinaryOperation("-",
					[](int64_t a, int64_t b){return a-b;},
					[](double a, double b){return a-b;}
					), value1, value2);
		case add:
			return std::visit(BinaryOperation("+",
					[](int64_t a, int64_t b){return a+b;},
					[](double a, double b){return a+b;},
					[](const std::string& a, const std::string& b){return a+b;}
			), value1, value2);
		case mult:
			return std::visit(BinaryOperation("*",
					[](int64_t a, int64_t b){return a*b;},
					[](double a, double b){return a*b;}
			), value1, value2);
		case divi:
			return std::visit(BinaryOperation("/",
					[](int64_t a, int64_t b){if(b==0) throw std::runtime_error("Division of int by zero"); else return a/b;},
					[](double a, double b){if(b==0.0) throw std::runtime_error("Division of double by zero"); else return a/b;}
			), value1, value2);
		case mod:
			return std::visit(BinaryOperation("%",
					[](int64_t a, int64_t b){if(b==0) throw std::runtime_error("Modulo of int by zero"); else return a%b;},
					[](double a, double b){if(b==0.0) throw std::runtime_error("Modulo of double by zero"); else return std::fmod(a,b);}
			), value1, value2);
		case eq:
			return std::visit(BinaryOperation("==",
					[](int64_t a, int64_t b){return (int64_t)(a==b);},
					[](double a, double b){return (int64_t)(a==b);}
			), value1, value2);
		case neq:
			return std::visit(BinaryOperation("!=",
					[](int64_t a, int64_t b){return (int64_t)(a!=b);},
					[](double a, double b){return (int64_t)(a!=b);}
			), value1, value2);
		case mor:
			return std::visit(BinaryOperation(">",
					[](int64_t a, int64_t b){return (int64_t)(a>b);},
					[](double a, double b){return (int64_t)(a>b);}
			), value1, value2);
		case meq:
			return std::visit(BinaryOperation(">=",
					[](int64_t a, int64_t b){return (int64_t)(a>=b);},
					[](double a, double b){return (int64_t)(a>=b);}
			), value1, value2);
		case les:
			return std::visit(BinaryOperation("<",
					[](int64_t a, int64_t b){return (int64_t)(a<b);},
					[](double a, double b){return (int64_t)(a<b);}
			), value1, value2);
		case leq:
			return std::visit(BinaryOperation("<=",
					[](int64_t a, int64_t b){return (int64_t)(a<=b);},
					[](double a, double b){return (int64_t)(a<=b);}
			), value1, value2);
		case Or:
			return std::visit(BinaryOperation("||",
					[](int64_t a, int64_t b){return (int64_t)(a||b);}
			), value1, value2);
		case And:
			return std::visit(BinaryOperation("&&",
					[](int64_t a, int64_t b){return (int64_t)(a&&b);}
			), value1, value2);
		default: //wrong operator (not possible)
			throw std::runtime_error("Illegal binary operation");
	}
}

InterpreterValue getUnaryOperationValue(const InterpreterValue& value, OperatorType type) {
	switch(type) {
		case negation:
			return std::visit(UnaryOperation("!",
					[](int64_t a){return (int64_t)!a;}
					), value);
			case minus:
				return std::visit(UnaryOperation("-",
						[](int64_t a){return (int64_t)-a;},
						[](double a){return (double)-a;}
				), value);
			case toIntConversion:
				return std::visit(UnaryOperation("(int)",
						[](int64_t a){return (int64_t)a;},
						[](double a){return (int64_t)a;}
				), value);
			case toDoubleConversion:
				return std::visit(UnaryOperation("(double)",
						[](int64_t a){return (double)a;},
						[](double a){return (double)a;}
				), value);
			default: //wrong operator (not possible)
				throw std::runtime_error("Illegal unary operation");
		}
	}
