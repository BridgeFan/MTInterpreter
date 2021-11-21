#pragma once

#include "DataSource.h"
#include <fstream>

class FileDataSource:public DataSource {
private:
	std::ifstream file;
	bool closed;
public:
	explicit FileDataSource(const std::string& path);
	virtual char getNextChar() override;
	virtual bool hasEnded() const;
	~FileDataSource();
};