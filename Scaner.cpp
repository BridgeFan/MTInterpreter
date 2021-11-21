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
#include "Token/Conversion.h"
#include "Token/StringToken.h"
#include "Token/ErrorToken.h"

void Scaner::getNextChar() {
	actualChar =source->getNextChar();
	internal+=actualChar;
	if(actualChar=='\n') {
		line++;
		col=1;
	}
	else
		col++;
}
std::unique_ptr<Token> Scaner::processString() {
	std::unique_ptr<Token> token(new StringToken(line, col-1, ""));
	while(true) {
		getNextChar();
		try {
			if (token->addChar(actualChar))
				break;
		}
		catch(std::bad_exception&) {
			return std::unique_ptr<Token>(new ErrorToken(token->getLine(),token->getColumn(),internal,token->getType(),ErrorType::overflow));
		}
		catch(...) {
			return std::unique_ptr<Token>(new ErrorToken(token->getLine(),token->getColumn(),internal,token->getType()));
		}
	}
	actualChar='\0';
	return token;
}

void Scaner::checkSpecialIds(std::unique_ptr<Token>& token) {
	if(typeid(*token)!=typeid(IdToken))
		return;
	std::string raw = dynamic_cast<IdToken *>(token.get())->getValue();
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
}

std::unique_ptr<Token> Scaner::processId() {
	std::unique_ptr<Token> token(new IdToken(line, col-1, {actualChar}));
	while(true) {
		getNextChar();
		try {
			if(token->addChar(actualChar))
				break;
		}
		catch(std::bad_exception&) {
			return std::unique_ptr<Token>(new ErrorToken(token->getLine(),token->getColumn(),internal,token->getType(),ErrorType::overflow));
		}
		catch(...) {
			return std::unique_ptr<Token>(new ErrorToken(token->getLine(),token->getColumn(),internal,token->getType()));
		}
	}
	checkSpecialIds(token);
	return token;
}

std::unique_ptr<Token> Scaner::processNumber(int64_t value, NumberState nState) {
	std::unique_ptr<Token> token(new Number(line, col-1, value, nState));
	while(true) {
		getNextChar();
		if(actualChar==eof)
			break;
		try {
			if (token->addChar(actualChar))
				break;
		}
		catch(std::bad_exception&) {
			return std::unique_ptr<Token>(new ErrorToken(token->getLine(),token->getColumn(),internal,token->getType(),ErrorType::overflow));
		}
		catch(...) {
			return std::unique_ptr<Token>(new ErrorToken(token->getLine(),token->getColumn(),internal,token->getType()));
		}
	}
	return token;
}

std::unique_ptr<Token> Scaner::getNextToken() {
	if(hasEnded())
		return {};
	if(actualChar=='\0') {
		getNextChar();
	}
	std::unique_ptr<Token> token;
	while(Token::isWhite(actualChar)) {
		getNextChar();
	}
	if(hasEnded() && actualChar==eof)
		return {};
	internal="";
	int retLine=line;
	int retCol=col;
	if(Token::isDefined(actualChar)) {
		if(actualChar==';') {
			actualChar='\0';
			return std::unique_ptr<Token>(new End(retLine, retCol));
		}
		if(actualChar=='{') {
			actualChar='\0';
			return std::unique_ptr<Token>(new BlockBegin(retLine, retCol));
		}
		if(actualChar=='}') {
			actualChar='\0';
			return std::unique_ptr<Token>(new BlockEnd(retLine, retCol));
		}
		if(actualChar==',') {
			actualChar='\0';
			return std::unique_ptr<Token>(new Comma(retLine, retCol));
		}
		if(actualChar=='!') {
			getNextChar();
			if(actualChar=='=') {
				actualChar = '\0';
				return std::unique_ptr<Token>(new RelOp(retLine, retCol, not_equal));
			}
			else
				return std::unique_ptr<Token>(new NegOp(retLine, retCol));
		}
		if(actualChar=='+') {
			getNextChar();
			if(actualChar=='=') {
				actualChar = '\0';
				return std::unique_ptr<Token>(new Assign(retLine, retCol, addAssign));
			}
			else
				return std::unique_ptr<Token>(new Add(retLine, retCol));
		}
		if(actualChar=='-') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new Assign(retLine, retCol, minusAssign));
			}
			else
				return std::unique_ptr<Token>(new Minus(retLine, retCol));
		}
		if(actualChar=='*') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new Assign(retLine, retCol, multAssign));
			}
			else
				return std::unique_ptr<Token>(new MultOp(retLine, retCol,multiply));
		}
		if(actualChar=='/') {
			getNextChar();
			if(actualChar=='/')
			{
				//single line comment
				while(actualChar!='\n') {
					getNextChar();
					if(actualChar==eof) {
						return std::unique_ptr<Token>(new ErrorToken(retLine, retCol, internal, Error_));
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
						return std::unique_ptr<Token>(new ErrorToken(retLine, retCol, internal, Error_));
					}
				}
				actualChar='\0';
				return getNextToken();
			}
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new Assign(retLine, retCol, divAssign));
			}
			else
				return std::unique_ptr<Token>(new MultOp(retLine, retCol,divide));
		}
		if(actualChar=='.') {
			return processNumber(0,plus_divide);
		}
		if(actualChar=='%') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new Assign(retLine, retCol, moduloAssign));
			}
			else
				return std::unique_ptr<Token>(new MultOp(retLine, retCol,modulo));
		}
		if(actualChar=='=') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new RelOp(retLine, retCol, equal));
			}
			else
				return std::unique_ptr<Token>(new Assign(retLine, retCol,assign));
		}
		if(actualChar=='<') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new RelOp(retLine, retCol, less_equal));
			}
			else
				return std::unique_ptr<Token>(new RelOp(retLine, retCol,less));
		}
		if(actualChar=='>') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new RelOp(retLine, retCol, more_equal));
			}
			else
				return std::unique_ptr<Token>(new RelOp(retLine, retCol,more));
		}
		if(actualChar=='|') {
			getNextChar();
			if(actualChar=='|') {
				actualChar='\0';
				return std::unique_ptr<Token>(new LogicOp(retLine, retCol, or_));
			}
			else {
				return std::unique_ptr<Token>(new ErrorToken(retLine, retCol, internal, Logic_));
			}
		}
		if(actualChar=='&') {
			getNextChar();
			if(actualChar=='&') {
				actualChar='\0';
				return std::unique_ptr<Token>(new LogicOp(retLine, retCol, and_));
			}
			else {
				return std::unique_ptr<Token>(new ErrorToken(retLine, retCol, internal, Logic_));
			}
		}
		if(actualChar=='(') {
			actualChar='0';
			return std::unique_ptr<Token>(new ParBegin(retLine,retCol));
		}
		if(actualChar==')') {
			actualChar='\0';
			return std::unique_ptr<Token>(new ParEnd(retLine,retCol));
		}
	}
	if(actualChar=='\"') {
		return processString();
	}
	if(Token::isLetter(actualChar)) {
		return processId();
	}
	else if(Token::isDigit(actualChar)) {
		return processNumber(actualChar-'0', plus);
	}
	else {
		return std::unique_ptr<Token>(new ErrorToken(retLine, retCol, {actualChar}, Error_));
	}
}

Scaner::Scaner(std::unique_ptr<DataSource> &&source) : source(std::move(source)) {}

bool Scaner::hasEnded() const {
	return source->hasEnded() && (actualChar=='\0' || actualChar==eof);
}
