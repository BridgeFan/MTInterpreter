//
// Created by kamil-hp on 18.01.2022.
//

#ifndef MTINTERPRETER_EVALUABLESYNTAXTREEELEMENT_H
#define MTINTERPRETER_EVALUABLESYNTAXTREEELEMENT_H

typedef std::variant<int64_t, double, std::nullptr_t, std::string> InterpreterValue;
class Visitor;

class EvaluableSyntaxTreeElement {
public:
	virtual InterpreterValue calculate(Visitor& visitor) = 0;
};

#endif //MTINTERPRETER_EVALUABLESYNTAXTREEELEMENT_H
