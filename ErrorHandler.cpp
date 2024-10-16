//
// Created by kamil-hp on 17.11.2021.
//

#include <iostream>
#include <map>
#include "ErrorHandler.h"
#include "SyntaxTree/InitNode.h"
#include "SyntaxTree/Expression.h"
std::map<TokenType, std::string> tokenTypeNames;
std::vector<std::pair<ErrorPlace, std::string> > ErrorHandler::errorInfo;
int ErrorHandler::limit = 1024;

int ErrorHandler::getErrorSize() {
	return errorInfo.size();
}

void ErrorHandler::addError(ErrorPlace place, const ErrorToken& token) {
	if(tokenTypeNames.empty()) {
		tokenTypeNames[TokenType::Error_]="Error";
		tokenTypeNames[TokenType::String_]="String";
		tokenTypeNames[TokenType::BlockEnd_]="BlockEnd";
		tokenTypeNames[TokenType::BlockBegin_]="BlockBegin";
		tokenTypeNames[TokenType::MultOp_]="MultOp";
		tokenTypeNames[TokenType::ParBegin_]="ParBegin";
		tokenTypeNames[TokenType::TypeName_]="TypeName";
		tokenTypeNames[TokenType::LoopMod_]="LoopMod";
		tokenTypeNames[TokenType::Add_]="Add";
		tokenTypeNames[TokenType::Assign_]="Assign";
		tokenTypeNames[TokenType::Comma_]="Comma";
		tokenTypeNames[TokenType::Conversion_]="Conversion";
		tokenTypeNames[TokenType::Else_]="Else";
		tokenTypeNames[TokenType::End_]="End";
		tokenTypeNames[TokenType::For_]="For";
		tokenTypeNames[TokenType::Id_]="Id";
		tokenTypeNames[TokenType::If_]="If";
		tokenTypeNames[TokenType::Logic_]="Logic";
		tokenTypeNames[TokenType::Minus_]="Minus";
		tokenTypeNames[TokenType::NegOp_]="NegOp";
		tokenTypeNames[TokenType::Number_]="Number";
		tokenTypeNames[TokenType::ParEnd_]="ParEnd";
		tokenTypeNames[TokenType::RelOp_]="RelOp";
		tokenTypeNames[TokenType::Return_]="Return";
		tokenTypeNames[TokenType::While_]="While";
	}
	std::string str;
	str+="ERROR (line="+std::to_string(token.getLine())+",col="+std::to_string(token.getColumn())+"): ";
	switch(token.getErrorType()) {
		case ErrorType::unexpectedCharacter:
			str+="Unexpected character (" + token.getValue() +") when trying to make token of ";
			str+=tokenTypeNames[token.getExpected()]+" type";
			break;
		case ErrorType::overflow:
			str+="Too great value of token of ";
			str+=tokenTypeNames[token.getExpected()]+" type";
			break;
		case ErrorType::unexpectedEof:
			str+="Unexpected end of file but expected ";
			str+=tokenTypeNames[token.getExpected()]+" type";
			break;
		case ErrorType::wrongToken:
			str+="Expected token of type " + tokenTypeNames[token.getExpected()] + " but got " + tokenTypeNames[token.getGotType()];
			break;
		case ErrorType::wrongEnd:
			str+="Wrong ending of expression";
			break;
		case ErrorType::modifyAssignToUninitialized:
			str+="Modify assign to unitialized variable";
			break;
		case ErrorType::unexpectedParEnd:
			str+="Unexpected )";
			break;
		case ErrorType::unexpectedParBegin:
			str+="Unexpected (";
			break;
		case ErrorType::emptyExpression:
			str+="Empty expression";
			break;
		case ErrorType::unexpectedEndOfExpression:
			str+="Unexpected end of expression. Too few arguments of binary operator " + tokenTypeNames[token.getExpected()];
			break;
	}
	errorInfo.emplace_back(place, str);
	if(errorInfo.size()>=limit) {
		showErrors(std::cout,std::cerr);
		std::cout << "Too many errors!";
		exit(EXIT_FAILURE);
	}
}

void ErrorHandler::showErrors(std::ostream& okOut, std::ostream& out) {
	if(getErrorSize()==0) {
		okOut << "No errors\n";
		return;
	}
	out << "Errors found: "+ std::to_string(ErrorHandler::getErrorSize())+"\n";
	for(auto&& [place, str]: errorInfo) {
		if(place==ScanerError)
			out << "Scaner error: ";
		else if(place==ParserError)
			out << "Parser error: ";
		else if(place==SemanticError)
			out << "Semantic error: ";
		else
			out << "Runtime error: ";
		out << str << "\n";
	}
}

void ErrorHandler::clear() {
	errorInfo.clear();
}

void ErrorHandler::setLimit(int a) {
	limit=a;
	if(errorInfo.size()>=limit) {
		showErrors(std::cout,std::cerr);
		std::cout << "Too many errors!";
		exit(EXIT_FAILURE);
	}
}

void ErrorHandler::addSemanticError(SemanticErrorType type, const std::string& func, const std::string& name) {
	std::string str;
	if(!func.empty())
		str="Function "+func+": ";
	switch(type) {
		case AlreadyDeclaredVariable:
			errorInfo.emplace_back(SemanticError, str+"Already declared variable " + name);
		case AlreadyDeclaredFunction:
			errorInfo.emplace_back(SemanticError, str+"Already declared function " + name);
		case NotDeclaredVariable:
			errorInfo.emplace_back(SemanticError, str+"Already declared variable " + name);
		case NotDeclaredFunction:
			errorInfo.emplace_back(SemanticError, str+"Already declared function " + name);
		case IllegalFunctionName:
			errorInfo.emplace_back(SemanticError, "Illegal function name " + name);
		case WrongMain:
			errorInfo.emplace_back(SemanticError,
								   "Main function not correct. It should have no parameter and return int");
	}
}

std::string toString(TypeType type) {
	switch(type) {
		case int_:
			return "int";
		case double_:
			return "double";
		case void_:
			return "void";
		case string_:
			return "string";
	}
	return "";
}

void ErrorHandler::addWrongAssignError(const std::string& func, const std::string& name, TypeType varType, TypeType gotType) {
	errorInfo.emplace_back(SemanticError, "Function "+func+": Wrong assigned type "+toString(gotType)+" ("+name+" type is "+toString(varType)+")");
}

void ErrorHandler::addWrongConditionError(const std::string &func, const std::string& name, TypeType gotType) {
	errorInfo.emplace_back(SemanticError, "Function "+func+": Wrong condition in "+name+" of type "+toString(gotType)+" (should be int)");
}

void ErrorHandler::addWrongParameterError(const std::string &func, const std::string &name, int index, TypeType varType, TypeType gotType) {
	errorInfo.emplace_back(SemanticError, "Function "+func+": Wrong parameter "+std::to_string(index)+" for calling function"+name+"Expected parameter of type "+toString(gotType)+" ("+name+" but got "+toString(varType)+")");
}

void ErrorHandler::addWrongReturnError(const std::string &func, TypeType varType, TypeType gotType) {
	errorInfo.emplace_back(SemanticError, "Function "+func+": Wrong return type "+toString(gotType)+" (should be "+toString(varType)+")");
}

void ErrorHandler::addWrongParameterError(const std::string &func, const std::string &name, int expectedSize, int gotSize) {
	errorInfo.emplace_back(SemanticError, "Function "+func+": Expected "+std::to_string(expectedSize)+" parameters but got"+std::to_string(gotSize));
}

std::string toString(OperatorType type) {
	switch(type) {
		case none: return "";
		case negation: return "!";
		case minus: return "-";
		case add: return "+";
		case mult: return "*";
		case divi: return "/";
		case mod: return "%";
		case eq: return "==";
		case neq: return "!=";
		case mor: return ">";
		case meq: return ">=";
		case les: return "<";
		case leq: return "<=";
		case Or: return "||";
		case And: return "&&";
		case toIntConversion: return "(int)";
		case toDoubleConversion: return "(double)";
	}
	return "";
}

std::string toString(LoopModT type) {
	switch(type) {
		case continueType: return "continue";
		case breakType: return "break";
	}
	return "";
}

void ErrorHandler::addIllegalOperationError(const std::string &func, OperatorType type, TypeType varType1,
                                            TypeType varType2) {
	errorInfo.emplace_back(SemanticError, "Function "+func+": Illegal operation "+toString(type)+" for parameter types "+toString(varType1)+" and "+toString(varType2));

}

void ErrorHandler::addIllegalOperationError(const std::string &func, OperatorType type, TypeType varType) {
	errorInfo.emplace_back(SemanticError, "Function "+func+": Illegal operation "+toString(type)+" for parameter type "+toString(varType));

}

void ErrorHandler::addUncoveredLoopMod(const std::string &func, LoopModT type) {
	errorInfo.emplace_back(SemanticError, "Function "+func+": Uncatched loop modifier "+toString(type));
}

