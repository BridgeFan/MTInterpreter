//
// Created by kamil-hp on 04.01.2022.
//

#ifndef MTINTERPRETER_SEMANTICEXPRESSIONTREE_H
#define MTINTERPRETER_SEMANTICEXPRESSIONTREE_H
#include <memory>
#include <cstdint>

enum TypeType: uint16_t;

class SemanticExpressionTree {
private:
public:
    std::shared_ptr<SemanticExpressionTree> left, right, parent;
	TypeType type=(TypeType)2u; //void_
	SemanticExpressionTree()=default;
	SemanticExpressionTree(TypeType type, SemanticExpressionTree* parent): type(type), parent(parent) {}
	void setLeft(TypeType lType) {left.reset(new SemanticExpressionTree(lType,this));}
	void unsetLeft() {left=nullptr;}
	[[nodiscard]] SemanticExpressionTree* getLeft() const {return left.get();}
	void setRight(TypeType rType) {right.reset(new SemanticExpressionTree(rType,this));}
	void unsetRight() {right=nullptr;}
    [[nodiscard]] SemanticExpressionTree* getRight() const {return right.get();}
    [[nodiscard]] SemanticExpressionTree* getParent() const {return parent.get();}
};


#endif //MTINTERPRETER_SEMANTICEXPRESSIONTREE_H
