//
// Created by kamil-hp on 04.01.2022.
//

#ifndef MTINTERPRETER_SYNTAXTREEELEMENT_H
#define MTINTERPRETER_SYNTAXTREEELEMENT_H
class Visitor;

class SyntaxTreeElement {
public:
	virtual void accept(Visitor& visitor) = 0;
};

#endif //MTINTERPRETER_SYNTAXTREEELEMENT_H
