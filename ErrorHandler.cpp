//
// Created by kamil-hp on 17.11.2021.
//

#include <iostream>
#include "ErrorHandler.h"

int ErrorHandler::errorCounter = 0;

void ErrorHandler::addError(ErrorType type, int line, int col, const std::string& raw, const std::string& desc) {
	std::cerr << "Error in line " + std::to_string(line) + ", column " + std::to_string(col) + " (" + raw + ")\n";
	std::cerr << "Error description: " << desc << "\n";
	errorCounter++;
	exit(1);
}

int ErrorHandler::getErrorSize() const {
	return errorCounter;
}
