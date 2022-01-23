#include <iostream>
#include <memory>
#include "DataSource/DataSource.h"
#include "DataSource/FileDataSource.h"
#include "DataSource/StringDataSource.h"
#include "Scaner.h"
#include "Token/ErrorToken.h"
#include "ErrorHandler.h"
#include "Parser.h"
#include "SyntaxTree/MappedSyntaxTree.h"
#include "Interpreter/Interpreter.h"

int main(int argc, char** argv) {
	//Conversion token will be created in parser, not in scaner to reduce wrong interpretation
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
	ErrorHandler::setLimit(100);
	Scaner scaner(std::move(dataSource));
	Parser parser(scaner);
	bool wereParserErrors = ErrorHandler::getErrorSize()>0;
	SyntaxTree tree = parser.parse();
	MappedSyntaxTree mapped;
	if(!mapped.mapTree(tree)) {
		if(wereParserErrors)
			std::cerr << "An error occured parsing file\n";
		else
			std::cerr << "An error occured mapping syntax tree\n";
		ErrorHandler::showErrors(std::cout, std::cerr);
		return 0;
	}
	std::cout << "No compilation errors. Ready for interpreting\n";
	Interpreter interpreter(std::cin, std::cout);
	int64_t returnedValue = interpreter.visitTree(mapped);
	std::cout << "Main function result was " << returnedValue;
	return 0;
}
