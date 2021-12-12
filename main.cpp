#include <iostream>
#include <memory>
#include "DataSource/DataSource.h"
#include "DataSource/FileDataSource.h"
#include "DataSource/StringDataSource.h"
#include "Scaner.h"
#include "Token/ErrorToken.h"
#include "ErrorHandler.h"
#include "Parser.h"

int main(int argc, char** argv) {
	//TODO: Conversion token will be created in parser, not in scaner to reduce wrong interpretation
	std::unique_ptr<DataSource> dataSource;
	/*if(argc<2) {
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
	}*/
	//dataSource = std::make_unique<StringDataSource>("int b,c;\ndouble e;\nint f(){}");
	ErrorHandler::setLimit(100);
	dataSource = std::make_unique<StringDataSource>("int a; void f(){}");
	Scaner scaner(std::move(dataSource));
	Parser parser(scaner);
	SyntaxTree tree = parser.parse();
	ErrorHandler::showErrors(std::cout,std::cerr);
	return 0;
}
