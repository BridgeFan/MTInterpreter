//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_ERRORHANDLER_H
#define MTINTERPRETER_ERRORHANDLER_H

#include <cstdint>
#include <string>
#include <vector>
#include "Token/ErrorToken.h"



class ErrorHandler {
	static std::vector<std::string> errorInfo;
public:
	static int getErrorSize();
	static void addScanerError(ErrorToken& token);
	static void showErrors(std::ostream& out);
	static void clear();
};


#endif //MTINTERPRETER_ERRORHANDLER_H
