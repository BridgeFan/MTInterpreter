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
	int line=0;
	int col=0;
	char actualChar='\0';
	std::string internal;
	std::unique_ptr<DataSource> source;
	void getNextChar();
	bool internalLoop(const std::string& str);
	std::unique_ptr<Token> processString();
	std::unique_ptr<Token> processId();
	std::unique_ptr<Token> processNumber(int64_t value, NumberState nState);
	void checkSpecialIds(std::unique_ptr<Token>& token);
public:
	explicit Scaner(std::unique_ptr<DataSource>&& source);
	std::unique_ptr<Token> getNextToken();
	[[nodiscard]] bool hasEnded() const;
};


#endif //MTINTERPRETER_SCANER_H
