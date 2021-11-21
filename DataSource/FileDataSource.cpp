//
// Created by kamil-hp on 17.11.2021.
//

#include "FileDataSource.h"

char FileDataSource::getNextChar() {
	if(closed)
		return eof;
	char a = (char)file.get();
	if(file.eof()) {
		file.close();
		closed=true;
		return eof;
	}
	return a;
}

FileDataSource::FileDataSource(const std::string &path) {
	file.open(path);
	if(!file.good())
		throw std::bad_exception();
	closed=false;
}

bool FileDataSource::hasEnded() const {
	return closed;
}

FileDataSource::~FileDataSource() {
	file.close();
}
