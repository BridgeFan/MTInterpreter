//
// Created by kamil-hp on 18.11.2021.
//

#include <cmath>
#include "Number.h"
#include "../ErrorHandler.h"

Number::Number(int line, int column, int64_t val, NumberState state) : Token(Number_, line, column), state(state) {
	value1=val;
}

std::variant<int64_t, double> Number::getValue() const {
	if(state==plus)
		return value1;
	return (double)value1+(double)value2/(double)std::pow(10.0,value2size);
}

bool Number::addChar(char c) {
	if(!isDigit(c)) {
		if(c=='.') {
			if(state==plus) {
				state=plus_divide;
				return false;
			}
			else {
				throw std::exception();
			}
		}
		else if(isLetter(c))
			throw std::exception();
		else
			return true;
	}
	int64_t a=c-'0';
	if((state==plus) && value1>(INT64_MAX-1)/10) {
		throw std::bad_exception();
	}
	else if(state==plus_divide && std::abs(value2)>(INT64_MAX-1)/10) {
		//ignore all next digits, because they have no signifance
		return false;
	}
	switch(state) {
		case plus:
			value1*=10;
			value1+=a;
			return false;
		case plus_divide:
			value2*=10;
			value2+=a;
			value2size++;
			return false;
	}
	return false;
}
