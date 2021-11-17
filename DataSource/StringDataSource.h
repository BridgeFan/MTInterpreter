//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_STRINGDATASOURCE_H
#define MTINTERPRETER_STRINGDATASOURCE_H


#include "DataSource.h"

class StringDataSource:public  DataSource {
	std::string source;
	int i;
public:
	explicit StringDataSource(std::string&& source);
	virtual char getNextChar() override;
	virtual bool hasEnded() const override;

};


#endif //MTINTERPRETER_STRINGDATASOURCE_H
