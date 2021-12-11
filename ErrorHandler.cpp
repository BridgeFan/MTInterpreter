//
// Created by kamil-hp on 17.11.2021.
//

#include <iostream>
#include <map>
#include "ErrorHandler.h"
std::map<TokenType, std::string> tokenTypeNames;
std::vector<std::pair<ErrorPlace, std::string> > ErrorHandler::errorInfo;

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
			break;
		case ErrorType::overflow:
			str+="Too great value of token of ";
			break;
		case ErrorType::unexpectedEof:
			str+="Unexpected end of file";
			break;
		case ErrorType::unexpectedToken:
			str+="Expected token of type " + tokenTypeNames[token.getExpected()] + " but got " + token.getValue();
			break;
	}
	str+=tokenTypeNames[token.getExpected()]+" type";
	errorInfo.emplace_back(place, str);
}

void ErrorHandler::showErrors(std::ostream& out) {
	if(getErrorSize()==0) {
		out << "No errors\n";
		return;
	}
	out << "Errors found: "+ std::to_string(ErrorHandler::getErrorSize())+"\n";
	for(auto&& [place, str]: errorInfo) {
		if(place==ScanerError)
			out << "Scaner error: ";
		if(place==ParserError)
			out << "Parser error: ";
		out << str << "\n";
	}
}

void ErrorHandler::clear() {
	errorInfo.clear();
}
