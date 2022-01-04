//
// Created by kamil-hp on 04.01.2022.
//
#include "InterpreterExpressionTree.h"
#include <iostream>

void divideByZeroExit() {
	std::cerr << "ERROR!!! Division by zero. Exit interpreter\n";
	exit(EXIT_FAILURE);
}

InterpreterValue getBinaryOperationValue(const InterpreterValue& value1, const InterpreterValue& value2, OperatorType type) {
	auto lValue = value1;
	auto rValue = value2;
	if(lValue.index()==string_ || rValue.index()==string_) {
		return std::get<string_>(lValue)+std::get<string_>(rValue);
	}
	if(lValue.index()==string_ || rValue.index()==int_) {
		return std::get<string_>(lValue)+std::to_string(std::get<int_>(rValue));
	}
	if(lValue.index()!=string_ || rValue.index()==double_) {
		return std::get<string_>(lValue)+std::to_string(std::get<double_>(rValue));
	}
	if(lValue.index()==int_ || rValue.index()==string_) {
		return std::to_string(std::get<int_>(lValue))+std::get<string_>(rValue);
	}
	if(lValue.index()==double_ || rValue.index()==string_) {
		return std::to_string(std::get<double_>(lValue))+std::get<string_>(rValue);
	}
	//numeric operations
	if(value1.index()==int_ && value2.index()==double_)
		lValue = (double)std::get<int_>(value1);
	if(value1.index()==double_ && value2.index()==int_)
		rValue = (double)std::get<int_>(value2);
	if(value1.index()==int_) {
		switch (type) {
			case minus:
				return std::get<int_>(value1)-std::get<int_>(value2);
			case add:
				return std::get<int_>(value1)+std::get<int_>(value2);
			case mult:
				return std::get<int_>(value1)*std::get<int_>(value2);
			case divi:
				if(std::get<int_>(value2)==0) {
					divideByZeroExit();
				}
				else
					return std::get<int_>(value1)/std::get<int_>(value2);
			case mod:
				if(std::get<int_>(value2)==0) {
					divideByZeroExit();
				}
				return std::get<int_>(value1)%std::get<int_>(value2);
			case eq:
				return std::get<int_>(value1)==std::get<int_>(value2);
			case neq:
				return std::get<int_>(value1)!=std::get<int_>(value2);
			case mor:
				return std::get<int_>(value1)>std::get<int_>(value2);
			case meq:
				return std::get<int_>(value1)>=std::get<int_>(value2);
			case les:
				return std::get<int_>(value1)<std::get<int_>(value2);
			case leq:
				return std::get<int_>(value1)<=std::get<int_>(value2);
			case Or:
				return std::get<int_>(value1)||std::get<int_>(value2);
			case And:
				return std::get<int_>(value1)&&std::get<int_>(value2);
			default: //wrong operator (not possible)
				return value1;
		}
	}
	else {
		switch (type) {
			case minus:
				return std::get<double_>(value1)-std::get<double_>(value2);
			case add:
				return std::get<double_>(value1)+std::get<double_>(value2);
			case mult:
				return std::get<double_>(value1)*std::get<double_>(value2);
			case divi:
				if(std::get<double_>(value2)==0.0) {
					divideByZeroExit();
				}
				else
					return std::get<int_>(value1)/std::get<int_>(value2);
			case mod:
				if(std::get<double_>(value2)==0.0) {
					divideByZeroExit();
				}
				return fmod(std::get<double_>(value1),std::get<double_>(value2));
			case eq:
				return std::get<double_>(value1)==std::get<double_>(value2);
			case neq:
				return std::get<double_>(value1)!=std::get<double_>(value2);
			case mor:
				return std::get<double_>(value1)>std::get<double_>(value2);
			case meq:
				return std::get<double_>(value1)>=std::get<double_>(value2);
			case les:
				return std::get<double_>(value1)<std::get<double_>(value2);
			case leq:
				return std::get<double_>(value1)<=std::get<double_>(value2);
			default: //wrong operator (not possible)
				return value1;
		}
	}
}

InterpreterValue getUnaryOperationValue(const InterpreterValue& value, OperatorType type) {
	if(value.index()==int_) {
		switch(type) {
			case negation:
				return !std::get<int_>(value);
			case minus:
				return -std::get<int_>(value);
			case toIntConversion:
				return (int64_t)std::get<int_>(value);
			case toDoubleConversion:
				return (double)std::get<int_>(value);
			default: //wrong operator (not possible)
				return value;
		}
	}
	else {
		switch(type) {
			case minus:
				return -std::get<double_>(value);
			case toIntConversion:
				return (int64_t)std::get<double_>(value);
			case toDoubleConversion:
				return (double)std::get<double_>(value);
			default: //wrong operator (not possible)
				return value;
		}
	}
}
