#pragma once

#include <string>

constexpr char eof = std::char_traits<char>::eof();

class DataSource {
public:
	virtual char getNextChar()=0;
	virtual bool hasEnded() const=0;
	virtual ~DataSource()=default;
};