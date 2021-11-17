//
// Created by kamil-hp on 17.11.2021.
//
#include "Token.h"
#include <utility>

Token::Token(int line, int column, std::string raw): line(line), column(column), raw(std::move(raw)) {}

TokenType Token::getType() const {
	return type;
}

void Token::finish() {

}

const std::string &Token::getRaw() const {
	return raw;
}

int Token::getColumn() const {
	return column;
}

int Token::getLine() const {
	return line;
}

bool Token::isLetter(char a) {
	return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a=='_';
}

bool Token::isDigit(char a) {
	return (a >= '0' && a <= '9');
}

bool Token::isWhite(char a) {
	return std::isspace(a);
}

bool Token::isDefined(char a) {
	const std::string defined="();,!-=+*/%|&{}";
	return defined.find(a) != std::string::npos;
}

bool If::addChar(char c) {
	return true;
}

If::If(int line, int column): Token(line, column, "if") {
	type=TokenType::If_;
}

bool While::addChar(char c) {
	return true;
}

While::While(int line, int column): Token(line, column, "while") {
	type=TokenType::While_;
}

bool For::addChar(char c) {
	return true;
}

For::For(int line, int column): Token(line, column, "while") {
	type=TokenType::For_;
}

Else::Else(int line, int column): Token(line, column, "else") {
	type=TokenType::Else_;
}

bool Else::addChar(char c) {
	return true;
}

bool ParBegin::addChar(char c) {
	return true;
}

ParBegin::ParBegin(int line, int column): Token(line, column, "(") {
	type = ParBegin_;
}

bool ParEnd::addChar(char c) {
	return true;
}

ParEnd::ParEnd(int line, int column): Token(line, column, ")") {
	type = ParEnd_;
}

bool End::addChar(char c) {
	return true;
}

End::End(int line, int column): Token(line, column, ";") {
	type = End_;
}

bool Comma::addChar(char c) {
	return true;
}

Comma::Comma(int line, int column): Token(line, column, ",") {
	type = Comma_;
}

bool NegOp::addChar(char c) {
	return true;
}

NegOp::NegOp(int line, int column): Token(line, column, "!") {
	type = NegOp_;
}

bool Minus::addChar(char c) {
	return true;
}

Minus::Minus(int line, int column): Token(line, column, "-") {
	type = Minus_;
}

bool Add::addChar(char c) {
	return true;
}

Add::Add(int line, int column): Token(line, column, "+") {
	type = Add_;
}

bool BlockBegin::addChar(char c) {
	return true;
}

BlockBegin::BlockBegin(int line, int column): Token(line, column, "{") {
	type = BlockBegin_;
}

bool BlockEnd::addChar(char c) {
	return true;
}

BlockEnd::BlockEnd(int line, int column): Token(line, column, "}") {
	type = BlockEnd_;
}

bool Return::addChar(char c) {
	return true;
}

Return::Return(int line, int column): Token(line, column, "return") {
	type = Return_;
}