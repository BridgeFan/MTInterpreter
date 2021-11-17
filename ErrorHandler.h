//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_ERRORHANDLER_H
#define MTINTERPRETER_ERRORHANDLER_H

#include <cstdint>
#include <string>

enum ErrorType: uint16_t {
	ScanerError
};

class ErrorHandler {
	static int errorCounter;
public:
	static int getErrorSize();
	static void addError(ErrorType type, int line, int col, const std::string& raw, const std::string& desc);
};


#endif //MTINTERPRETER_ERRORHANDLER_H
