//
// Created by kamil-hp on 17.11.2021.
//

#include "Scaner.h"
#include "Token/IdToken.h"
#include "Token/LoopMod.h"
#include "Token/TypeName.h"
#include "Token/Assign.h"
#include "Token/MultOp.h"
#include "ErrorHandler.h"
#include "Token/RelOp.h"
#include "Token/LogicOp.h"

void Scaner::getNextChar() {
	actualChar =source->getNextChar();
	if(actualChar=='\n') {
		line++;
		col=0;
	}
	else
		col++;
}

std::unique_ptr<Token> Scaner::getNextToken() {
	if(actualChar=='\0') {
		getNextChar();
	}
	std::unique_ptr<Token> token;
	while(Token::isWhite(actualChar)) {
		getNextChar();
	}
	if(Token::isDefined(actualChar)) {
		if(actualChar==';') {
			actualChar='\0';
			return std::unique_ptr<Token>(new End(line, col));
		}
		if(actualChar=='{') {
			actualChar='\0';
			return std::unique_ptr<Token>(new BlockBegin(line, col));
		}
		if(actualChar=='}') {
			actualChar='\0';
			return std::unique_ptr<Token>(new BlockEnd(line, col));
		}
		if(actualChar==',') {
			actualChar='\0';
			return std::unique_ptr<Token>(new Comma(line, col));
		}
		if(actualChar=='!') {
			getNextChar();
			if(actualChar=='=') {
				actualChar = '\0';
				return std::unique_ptr<Token>(new RelOp(line, col, not_equal));
			}
			else
				return std::unique_ptr<Token>(new NegOp(line, col));
		}
		if(actualChar=='+') {
			getNextChar();
			if(actualChar=='=') {
				actualChar = '\0';
				return std::unique_ptr<Token>(new Assign(line, col, addAssign));
			}
			else
				return std::unique_ptr<Token>(new Add(line, col));
		}
		if(actualChar=='-') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new Assign(line, col, minusAssign));
			}
			else
				return std::unique_ptr<Token>(new Minus(line, col));
		}
		if(actualChar=='*') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new Assign(line, col, multAssign));
			}
			else
				return std::unique_ptr<Token>(new MultOp(line, col,multiply));
		}
		if(actualChar=='/') {
			getNextChar();
			if(actualChar=='/')
			{
				//single line comment
				while(actualChar!='\n') {
					getNextChar();
					if(actualChar==eof) {
						ErrorHandler::addError(ScanerError,line,col,"","Unexpected end of file error");
						exit(0);
					}
				}
				actualChar='\0';
				return getNextToken();
			}
			if(actualChar=='*')
			{
				//multi line comment
				char prevChar='\0';
				while(prevChar!='*' && actualChar!='/') {
					prevChar=actualChar;
					getNextChar();
					if(actualChar==eof) {
						ErrorHandler::addError(ScanerError,line,col,"","Unexpected end of file error");
						exit(0);
					}
				}
				actualChar='\0';
				return getNextToken();
			}
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new Assign(line, col, divAssign));
			}
			else
				return std::unique_ptr<Token>(new MultOp(line, col,divide));
		}
		if(actualChar=='%') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new Assign(line, col, moduloAssign));
			}
			else
				return std::unique_ptr<Token>(new MultOp(line, col,modulo));
		}
		if(actualChar=='=') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new RelOp(line, col, equal));
			}
			else
				return std::unique_ptr<Token>(new Assign(line, col,assign));
		}
		if(actualChar=='<') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new RelOp(line, col, less_equal));
			}
			else
				return std::unique_ptr<Token>(new RelOp(line, col,less));
		}
		if(actualChar=='>') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new RelOp(line, col, more_equal));
			}
			else
				return std::unique_ptr<Token>(new RelOp(line, col,more));
		}
		if(actualChar=='|') {
			getNextChar();
			if(actualChar=='|') {
				actualChar='\0';
				return std::unique_ptr<Token>(new LogicOp(line, col, or_));
			}
			else {
				return {}; //TODO: error
			}
		}
		if(actualChar=='&') {
			getNextChar();
			if(actualChar=='&') {
				actualChar='\0';
				return std::unique_ptr<Token>(new LogicOp(line, col, and_));
			}
			else {
				return {}; //TODO: error
			}
		}
		if(actualChar=='(') {
			getNextChar();
			if(actualChar=='i' || actualChar=='d') {
				//TODO: conversion
				actualChar='\0';
				return std::unique_ptr<Token>(new LogicOp(line, col, and_));
			}
			else {
				return std::unique_ptr<Token>(new ParBegin(line,col));
			}
		}
		if(actualChar==')') {
			actualChar='\0';
			return std::unique_ptr<Token>(new ParEnd(line,col));
		}

	}
	if(Token::isLetter(actualChar)) {
		token.reset(new IdToken(line,col,{actualChar})); //id
	}
	//only for Number, IdToken
	while(true) {
		getNextChar();
		if(actualChar==eof) {
			ErrorHandler::addError(ScanerError,line,col,"","Unexpected end of file error");
			exit(0);
		}
		if(token->addChar(actualChar))
			break;
	}
	std::string raw=token->getRaw();
	//check special keywords
	if(raw=="if") {
		token.reset(new If(token->getLine(),token->getColumn()));
	}
	if(raw=="else") {
		token.reset(new Else(token->getLine(),token->getColumn()));
	}
	if(raw=="while") {
		token.reset(new While(token->getLine(),token->getColumn()));
	}
	if(raw=="for") {
		token.reset(new For(token->getLine(),token->getColumn()));
	}
	if(raw=="return") {
		token.reset(new Return(token->getLine(),token->getColumn()));
	}
	if(raw=="continue") {
		token.reset(new LoopMod(token->getLine(),token->getColumn(),continueMod));
	}
	if(raw=="break") {
		token.reset(new LoopMod(token->getLine(),token->getColumn(),breakMod));
	}
	if(raw=="int") {
		token.reset(new TypeName(token->getLine(),token->getColumn(),intType));
	}
	if(raw=="double") {
		token.reset(new TypeName(token->getLine(),token->getColumn(),doubleType));
	}
	token->finish();
	return token;
}

Scaner::Scaner(std::unique_ptr<DataSource> &&source) : source(std::move(source)) {}

bool Scaner::hasEnded() const {
	return source->hasEnded();
}
