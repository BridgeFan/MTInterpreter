//
// Created by kamil-hp on 21.11.2021.
//

#ifndef MTINTERPRETER_TESTUTILS_H
#define MTINTERPRETER_TESTUTILS_H
#include "../Scaner.h"
#include "../Token/StringToken.h"
#include "../Token/IdToken.h"
#include "../Parser.h"
#include "../SyntaxTree/MappedSyntaxTree.h"

Scaner initScaner(std::string&& value) {
	std::unique_ptr<DataSource> source = std::make_unique<StringDataSource>(std::move(value));
	return Scaner(std::move(source));
}

bool isTokenTypeCorrect(Token& token, TokenType type) {
	return token.getType()==type;
}

template<typename T>
bool isTokenSubtypeCorrect(T& token, uint16_t subtype) {
	try {
		return token.getSubType()==subtype;
	}
	catch(...) {
		return false;
	}
}

bool isIntValueCorrect(Number& token, int64_t value) {
	auto val = token.getValue();
	if(val.index()!=0)
		return false;
	return value == std::get<0>(val);
}

bool isDoubleValueCorrect(Number& token, double value) {
	auto val = token.getValue();
	if(val.index()!=1)
		return false;
	return std::abs(value - std::get<1>(val)) < 1e-6;
}

bool isIdValueCorrect(IdToken& token, const std::string& value) {
	return value == token.getValue();
}

bool isStringValueCorrect(StringToken& token, const std::string& value) {
	return value == token.getValue();
}

std::pair<bool, MappedSyntaxTree> initMappedSyntaxTree(std::string&& a) {
    Scaner scaner = initScaner(std::move(a));
    Parser parser(scaner);
    SyntaxTree result = parser.parse();
    MappedSyntaxTree tree;
    bool wasGood = tree.mapTree(result);
    return {wasGood, tree};
}

#endif //MTINTERPRETER_TESTUTILS_H
