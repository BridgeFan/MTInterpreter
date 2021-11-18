//
// Created by kamil-hp on 17.11.2021.
//

#include <iostream>
#include "ErrorHandler.h"

int ErrorHandler::getErrorSize() {
	return errorInfo.size();
}

void ErrorHandler::addScanerError(ErrorToken &token) {
	//TODO
}

void ErrorHandler::showErrors() {
	if(getErrorSize()==0) {
		std::cout << "No errors\n";
	}
	std::cout << "Errors found: "+ std::to_string(getErrorSize())+"\n";
	for(const std:: string& s: errorInfo) {
		std::cout << s << "\n";
	}
}
