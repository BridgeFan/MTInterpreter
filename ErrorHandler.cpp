//
// Created by kamil-hp on 17.11.2021.
//

#include <iostream>
#include <map>
#include "ErrorHandler.h"
std::map<TokenType, std::string> tokenTypeNames;
std::vector<std::string> ErrorHandler::errorInfo;

int ErrorHandler::getErrorSize() {
	return errorInfo.size();
}

void ErrorHandler::addScanerError(ErrorToken &token) {
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
			break;
		case ErrorType::overflow:
			str+="Too great value of token of ";
			break;
	}
	str+=tokenTypeNames[token.getExpected()]+" type";
	errorInfo.push_back(str);
}

void ErrorHandler::showErrors() {
	if(getErrorSize()==0) {
		std::cout << "No errors\n";
		return;
	}
	std::cout << "Errors found: "+ std::to_string(ErrorHandler::getErrorSize())+"\n";
	for(const std:: string& s: errorInfo) {
		std::cout << s << "\n";
	}
}
