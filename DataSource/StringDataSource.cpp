//
// Created by kamil-hp on 17.11.2021.
//

#include "StringDataSource.h"

StringDataSource::StringDataSource(std::string &&source): source(std::move(source)), i(0) {
}

char StringDataSource::getNextChar() {
	if(hasEnded())
		return eof;
	char a = source[i];
	i++;
	return a;
}

bool StringDataSource::hasEnded() const {
	return i>=source.length();
}
