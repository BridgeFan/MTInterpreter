//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_SCANER_H
#define MTINTERPRETER_SCANER_H

#include <memory>
#include "Token/Token.h"
#include "DataSource/DataSource.h"

class Scaner {
private:
	int line=0;
	int col=0;
	char actualChar='\0';
	std::unique_ptr<DataSource> source;
	void getNextChar();
public:
	explicit Scaner(std::unique_ptr<DataSource>&& source);
	std::unique_ptr<Token> getNextToken();
	[[nodiscard]] bool hasEnded() const;
};


#endif //MTINTERPRETER_SCANER_H
