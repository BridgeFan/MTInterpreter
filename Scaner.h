//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_SCANER_H
#define MTINTERPRETER_SCANER_H

#include <memory>
#include "Token/Token.h"
#include "DataSource/DataSource.h"
#include "Token/Number.h"

class Scaner {
private:
	int line=1;
	int col=1;
	char actualChar='\0';
	std::string internal;
	std::unique_ptr<DataSource> source;
	void getNextChar();
	std::unique_ptr<Token> processString();
	std::unique_ptr<Token> processId();
	std::unique_ptr<Token> processNumber();
	std::unique_ptr<Token> checkSpecialIds(int l, int c, const std::string& val);
public:
	explicit Scaner(std::unique_ptr<DataSource>&& source);
	virtual std::unique_ptr<Token> getNextToken();
	virtual bool hasEnded() const;
	virtual int getLine() const {return line;}
	virtual int getColumn() const {return col;}
};


#endif //MTINTERPRETER_SCANER_H
