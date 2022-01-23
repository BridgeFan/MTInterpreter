//
// Created by kamil-hp on 11.12.2021.
//

#ifndef MTINTERPRETER_TOKENSOURCE_H
#define MTINTERPRETER_TOKENSOURCE_H
#include "Scaner.h"
#include "DataSource/StringDataSource.h"
#include <vector>
#include <memory>

class TokenSource: public Scaner {
private:
	std::vector<Token> tokens;
	int id;
public:
	TokenSource(std::vector<Token> tokens): Scaner(std::make_unique<StringDataSource>("")), tokens(tokens), id(0) {}
	virtual std::unique_ptr<Token> getNextToken() {
		if(hasEnded())
			return nullptr;
		auto tokenPtr = std::make_unique<Token>(tokens[id]);
		id++;
		return tokenPtr;
	}
	virtual bool hasEnded() const {return id<tokens.size();}
	virtual int getLine() const {return id/10;}
	virtual int getColumn() const {return id%10;}
};

#endif //MTINTERPRETER_TOKENSOURCE_H
