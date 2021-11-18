#include <iostream>
#include <memory>
#include "DataSource/DataSource.h"
#include "DataSource/FileDataSource.h"
#include "DataSource/StringDataSource.h"
#include "Scaner.h"
#include "Token/ErrorToken.h"
#include "ErrorHandler.h"

int main(int argc, char** argv) {
	std::unique_ptr<DataSource> dataSource;
	if(argc<2) {
		std::cout << "Usage:\n";
		std::cout << "file: provide file path in second parameter\n";
		std::cout << "str: provide string in second parameter\n";
		return 0;
	}
	else {
		if(argv[1]==std::string("file")) {
			if(argc<2) {
				std::cout << "File path not provided. Exit program\n";
				return 0;
			}
			else {
				dataSource = std::make_unique<FileDataSource>(argv[2]);
			}
		}
		else if(argv[1]==std::string("str")) {
			if(argc<2) {
				std::cout << "String not provided. Exit program\n";
				return 0;
			}
			else {
				dataSource = std::make_unique<StringDataSource>(argv[2]);
			}
		}
	}
	Scaner scaner(std::move(dataSource));
	auto tokenPtr = scaner.getNextToken();
	if(tokenPtr->getType()==Error_) {
		auto* token = dynamic_cast<ErrorToken*>(tokenPtr.get());
		ErrorHandler::addScanerError(*token);
	}
	std::cout << "Hello, World!" << std::endl;
	return 0;
}
