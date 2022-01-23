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
		std::string code;
		std::cout << "Type source code:\n";
		while(true) {
			std::string line;
			std::getline(std::cin, line);
			if(line.empty()) {
				std::cout << "Would you like to finish code. Y-yes, N-no: ";
				char a;
				std::cin >> a;
				if(a=='Y' || a=='y')
					break;
			}
			code+=line+"\n";
		}
		//string
		std::cout << "Usage:\n";
		std::cout << "file: provide file path in second parameter\n";
		std::cout << "str: provide string in second parameter\n";
		return 0;
	}
	else {
		dataSource = std::make_unique<FileDataSource>(argv[1]);
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
