//
// Created by kamil-hp on 17.11.2021.
//

#ifndef MTINTERPRETER_ERRORHANDLER_H
#define MTINTERPRETER_ERRORHANDLER_H

#include <cstdint>
#include <string>
#include <vector>
#include "Token/ErrorToken.h"

enum ErrorPlace: uint16_t {
	ScanerError,
	ParserError,
	SemanticError,
	RuntimeError
};

enum SemanticErrorType: uint16_t {
	AlreadyDeclaredVariable,
	AlreadyDeclaredFunction,
	IllegalFunctionName,
	WrongMain,
	NotDeclaredVariable,
	NotDeclaredFunction
};
enum TypeType: uint16_t;
enum OperatorType: uint16_t;
enum LoopModT: uint16_t;

class ErrorHandler {
	static int limit;
	static std::vector<std::pair<ErrorPlace, std::string> > errorInfo;
public:
	static int getErrorSize();
	static void addError(ErrorPlace place, const ErrorToken& token);
	static void addSemanticError(SemanticErrorType type, const std::string& func, const std::string& name);
	static void addWrongAssignError(const std::string& func, const std::string& name, TypeType varType, TypeType gotType);
	static void addIllegalOperationError(const std::string& func, OperatorType op, TypeType varType1, TypeType varType2);
	static void addIllegalOperationError(const std::string& func, OperatorType op, TypeType varType);
	static void addUncoveredLoopMod(const std::string& func, LoopModT type);
	static void addWrongConditionError(const std::string& func, const std::string& name, TypeType gotType);
	static void addWrongParameterError(const std::string& func, const std::string& name, int expectedSize, int gotSize);
	static void addWrongParameterError(const std::string& func, const std::string& name, int index, TypeType varType, TypeType gotType);
	static void addWrongReturnError(const std::string& func, TypeType varType, TypeType gotType);
	static void addSemanticError(SemanticErrorType type, const std::string& name) {addSemanticError(type,"",name);}
	static void showErrors(std::ostream& okOut, std::ostream& out);
	static void clear();
	static void setLimit(int a);
};


#endif //MTINTERPRETER_ERRORHANDLER_H
