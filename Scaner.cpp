//
// Created by kamil-hp on 17.11.2021.
//

#include "Scaner.h"

#include <memory>
#include <map>
#include <cmath>
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
#include "Token/Util.h"
std::map<char, char> stringSpecialChars = {{'\\','\\'},{'\'','\''},{'\"','\"'},{'\?','\?'},
										   {'a','\a'},{'b','\b'},{'f','\f'},{'n','\n'},{'r','\r'},
										   {'t','\t'},{'v','\v'}};

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
	int l=line, c=col;
	std::string val;
	getNextChar();
	while(actualChar!='\"') {
		if(val.length()>=Util::maxLength)
			return std::unique_ptr<Token>(new ErrorToken(l,c,internal,String_,ErrorType::overflow));
		if(actualChar=='\\') {
			getNextChar();
			if(stringSpecialChars.find(actualChar)!=stringSpecialChars.end())
				val += stringSpecialChars[actualChar];
			else
				return std::unique_ptr<Token>(new ErrorToken(l,c,internal,String_));
		}
		else if (Util::isStringCompatible(actualChar)) {
			val += actualChar;
		}
		else if(actualChar!='\"') {
			//error
			val += actualChar;
			return std::unique_ptr<Token>(new ErrorToken(l,c,internal,String_));
		}
		getNextChar();
	}
	return std::make_unique<StringToken>(l, c-1, val);
}

std::unique_ptr<Token> Scaner::checkSpecialIds(int l, int c, const std::string& val) {
	//check special keywords
	if(val=="if") {
		return std::make_unique<Token>(TokenType::If_, l, c);
	}
	if(val=="else") {
		return std::make_unique<Token>(TokenType::Else_, l, c);
	}
	if(val=="while") {
		return std::make_unique<Token>(TokenType::While_, l, c);
	}
	if(val=="for") {
		return std::make_unique<Token>(TokenType::For_, l, c);
	}
	if(val=="return") {
		return std::make_unique<Token>(TokenType::Return_, l, c);
	}
	if(val=="continue") {
		return std::make_unique<LoopMod>(l,c,continueMod);
	}
	if(val=="break") {
		return std::make_unique<LoopMod>(l,c,breakMod);
	}
	if(val=="int") {
		return std::make_unique<TypeName>(l,c,intType);
	}
	if(val=="double") {
		return std::make_unique<TypeName>(l,c,doubleType);
	}
	if(val=="void") { //needed for function definition
		return std::make_unique<Token>(Void_, l,c);
	}
	return std::make_unique<IdToken>(l,c,val);
}

std::unique_ptr<Token> Scaner::processId() {
	int l=line;
	int c=col-1;
	std::string id={actualChar};
	while(actualChar!=eof) {
		getNextChar();
		if(id.length()>=Util::maxLength)
			return std::unique_ptr<Token>(new ErrorToken(l,c,internal,Id_,ErrorType::overflow));
		if (Util::isLetter(actualChar) || Util::isDigit(actualChar))
			id += actualChar;
		else if (Util::isDefined(actualChar) || Util::isWhite(actualChar) || actualChar==eof)
			break;
		else
			return std::unique_ptr<Token>(new ErrorToken(l,c,internal,Id_));
	}
	return checkSpecialIds(l,c,id);
}

std::unique_ptr<Token> Scaner::processNumber() {
	int l=line;
	int c=col-1;
	int64_t val1 = 0;
	int64_t val2 = 0;
	int val2Pos = 0;
	//first loop
	while(Util::isDigit(actualChar)) {
		int64_t a=actualChar-'0';
		if(val1>(INT64_MAX-1)/10) {
			return std::make_unique<ErrorToken>(l,c,internal,Number_,ErrorType::overflow);
		}
		val1*=10;
		val1+=a;
		getNextChar();
	}
	if(Util::isLetter(actualChar))
		return std::make_unique<ErrorToken>(l,c,internal,Number_);
	if(actualChar!='.')
		return std::make_unique<Number>(l,c,val1);
	getNextChar();
	//second loop
	while(Util::isDigit(actualChar)) {
		int64_t b=actualChar-'0';
		if(val2>(INT64_MAX-1)/10) {
			getNextChar();
			continue;
		}
		val2*=10;
		val2+=b;
		val2Pos++;
		getNextChar();
	}
	if(Util::isLetter(actualChar))
		return std::make_unique<ErrorToken>(l,c,internal,Number_);
	double val3=(double)val1+((double)val2/std::pow(10.0,val2Pos));
	return std::make_unique<Number>(l,c,val3);
}

std::unique_ptr<Token> Scaner::getNextToken() {
	/*if(hasEnded())
		return {};*/
	if(actualChar=='\0') {
		getNextChar();
	}
	std::unique_ptr<Token> token;
	while(Util::isWhite(actualChar)) {
		getNextChar();
	}
	if(hasEnded() && actualChar==eof)
		return std::make_unique<Token>(EndOfFile, line, col); //EndOfFile token
	internal="";
	int retLine=line;
	int retCol=col;
	/*if(Token::isDefined(actualChar))*/ {
		if(actualChar==';') {
			actualChar='\0';
			return std::make_unique<Token>(End_, retLine, retCol);
		}
		if(actualChar=='{') {
			actualChar='\0';
			return std::make_unique<Token>(BlockBegin_, retLine, retCol);
		}
		if(actualChar=='}') {
			actualChar='\0';
			return std::make_unique<Token>(BlockEnd_, retLine, retCol);
		}
		if(actualChar==',') {
			actualChar='\0';
			return std::make_unique<Token>(Comma_, retLine, retCol);
		}
		if(actualChar=='!') {
			getNextChar();
			if(actualChar=='=') {
				actualChar = '\0';
				return std::unique_ptr<Token>(new RelOp(retLine, retCol, not_equal));
			}
			else
				return std::make_unique<Token>(NegOp_, retLine, retCol);
		}
		if(actualChar=='+') {
			getNextChar();
			if(actualChar=='=') {
				actualChar = '\0';
				return std::unique_ptr<Token>(new Assign(retLine, retCol, addAssign));
			}
			else
				return std::make_unique<Token>(Add_, retLine, retCol);
		}
		if(actualChar=='-') {
			getNextChar();
			if(actualChar=='=') {
				actualChar='\0';
				return std::unique_ptr<Token>(new Assign(retLine, retCol, minusAssign));
			}
			else
				return std::make_unique<Token>(Minus_, retLine, retCol);
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
			return processNumber();
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
			actualChar='\0';
			return std::make_unique<Token>(ParBegin_, retLine,retCol);
		}
		if(actualChar==')') {
			actualChar='\0';
			return std::make_unique<Token>(ParEnd_, retLine,retCol);
		}
	}
	if(actualChar=='\"') {
		return processString();
	}
	if(Util::isLetter(actualChar)) {
		return processId();
	}
	else if(Util::isDigit(actualChar)) {
		return processNumber();
	}
	else {
		return std::unique_ptr<Token>(new ErrorToken(retLine, retCol, {actualChar}, Error_));
	}
}

Scaner::Scaner(std::unique_ptr<DataSource> &&source) : source(std::move(source)) {}

bool Scaner::hasEnded() const {
	return source->hasEnded() && (actualChar=='\0' || actualChar==eof);
}
