//
// Created by kamil-hp on 21.11.2021.
//

#include <gtest/gtest.h>
#include "../DataSource/FileDataSource.h"
#include "../DataSource/StringDataSource.h"
#include "TestUtils.h"
#include "../Token/LoopMod.h"
#include "../Token/TypeName.h"
#include "../Token/Assign.h"
#include "../Token/MultOp.h"
#include "../Token/RelOp.h"
#include "../Token/LogicOp.h"
#include "../Token/Conversion.h"
#include "../Token/ErrorToken.h"
#include "../Token/LoopMod.h"
#include "../ErrorHandler.h"
#include "../Parser.h"
#include "../Interpreter/Interpreter.h"

//Data source tests
TEST(DataSourceTest, File) {
	DataSource* dataSource = new FileDataSource("example.txt");
	ASSERT_EQ(dataSource->getNextChar(),'a');
	ASSERT_EQ(dataSource->getNextChar(),'9');
	ASSERT_EQ(dataSource->getNextChar(),';');
	ASSERT_EQ(dataSource->getNextChar(),'\n');
	EXPECT_EQ(dataSource->getNextChar(),eof);
	delete dataSource;
}
TEST(DataSourceTest, FileEmpty) {
	DataSource* dataSource = new FileDataSource("empty.txt");
	ASSERT_EQ(dataSource->getNextChar(),eof);
	delete dataSource;
}

TEST(DataSourceTest, String) {
	DataSource* dataSource = new StringDataSource("a9;");
	ASSERT_EQ(dataSource->getNextChar(),'a');
	ASSERT_EQ(dataSource->getNextChar(),'9');
	ASSERT_EQ(dataSource->getNextChar(),';');
	ASSERT_EQ(dataSource->getNextChar(),eof);
	ASSERT_EQ(dataSource->getNextChar(),eof);
	delete dataSource;
}

//Scaner tests
TEST(ScanerTest, Empty) {
	Scaner scaner = initScaner("");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::EndOfFile);
}

TEST(ScanerTest, White) {
	Scaner scaner = initScaner("   \t\t\n");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::EndOfFile);
}

TEST(ScanerTest, Id1) {
	Scaner scaner = initScaner("aisu");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Id_);
	auto idTokenPtr = dynamic_cast<IdToken*>(tokenPtr.get());
	ASSERT_EQ(idTokenPtr->getValue(),"aisu");
}

TEST(ScanerTest, Id2) {
	Scaner scaner = initScaner(" sa");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Id_);
	auto idTokenPtr = dynamic_cast<IdToken*>(tokenPtr.get());
	ASSERT_EQ(idTokenPtr->getValue(),"sa");
}

TEST(ScanerTest, Id3) {
	Scaner scaner = initScaner("k9_a");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Id_);
	auto idTokenPtr = dynamic_cast<IdToken*>(tokenPtr.get());
	ASSERT_EQ(idTokenPtr->getValue(),"k9_a");
}

TEST(ScanerTest, If) {
	Scaner scaner = initScaner("if");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::If_);
}

TEST(ScanerTest, Else) {
	Scaner scaner = initScaner("else\n");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Else_);
}

TEST(ScanerTest, While) {
	Scaner scaner = initScaner("while ");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::While_);
}

TEST(ScanerTest, For) {
	Scaner scaner = initScaner("for");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::For_);
}

TEST(ScanerTest, Continue) {
	Scaner scaner = initScaner("continue");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::LoopMod_);
	auto loopPtr = dynamic_cast<LoopMod*>(tokenPtr.get());
	ASSERT_EQ(loopPtr->getSubtype(),continueMod);
}

TEST(ScanerTest, Break) {
	Scaner scaner = initScaner("break");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::LoopMod_);
	auto loopPtr = dynamic_cast<LoopMod*>(tokenPtr.get());
	ASSERT_EQ(loopPtr->getSubtype(),breakMod);
}

TEST(ScanerTest, Int) {
	Scaner scaner = initScaner("int");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::TypeName_);
	auto loopPtr = dynamic_cast<TypeName*>(tokenPtr.get());
	ASSERT_EQ(loopPtr->getSubtype(),intType);
}

TEST(ScanerTest, Double) {
	Scaner scaner = initScaner("double");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::TypeName_);
	auto loopPtr = dynamic_cast<TypeName*>(tokenPtr.get());
	ASSERT_EQ(loopPtr->getSubtype(),doubleType);
}

TEST(ScanerTest, ParBegin) {
	Scaner scaner = initScaner("(");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::ParBegin_);
}

TEST(ScanerTest, ParEnd) {
	Scaner scaner = initScaner(")");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::ParEnd_);
}

TEST(ScanerTest, End) {
	Scaner scaner = initScaner(";");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::End_);
}

TEST(ScanerTest, Comma) {
	Scaner scaner = initScaner(",");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Comma_);
}

TEST(ScanerTest, NegOp) {
	Scaner scaner = initScaner("!");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::NegOp_);
}

TEST(ScanerTest, Minus) {
	Scaner scaner = initScaner("-");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Minus_);
}

TEST(ScanerTest, Add) {
	Scaner scaner = initScaner("+");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Add_);
}

TEST(ScanerTest, Return) {
	Scaner scaner = initScaner("return");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Return_);
}

TEST(ScanerTest, Assign) {
	Scaner scaner = initScaner("=");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Assign_);
	auto asPtr = dynamic_cast<Assign*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),AssignType::assign);
}

TEST(ScanerTest, AddAssign) {
	Scaner scaner = initScaner("+=");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Assign_);
	auto asPtr = dynamic_cast<Assign*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),AssignType::addAssign);
}

TEST(ScanerTest, MinusAssign) {
	Scaner scaner = initScaner("-=");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Assign_);
	auto asPtr = dynamic_cast<Assign*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),AssignType::minusAssign);
}

TEST(ScanerTest, MultAssign) {
	Scaner scaner = initScaner("*=");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Assign_);
	auto asPtr = dynamic_cast<Assign*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),AssignType::multAssign);
}

TEST(ScanerTest, divAssign) {
	Scaner scaner = initScaner("/=");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Assign_);
	auto asPtr = dynamic_cast<Assign*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),AssignType::divAssign);
}

TEST(ScanerTest, modAssign) {
	Scaner scaner = initScaner("%=");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Assign_);
	auto asPtr = dynamic_cast<Assign*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),AssignType::moduloAssign);
}

TEST(ScanerTest, mult) {
	Scaner scaner = initScaner("*");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::MultOp_);
	auto asPtr = dynamic_cast<MultOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),MultOpType::multiply);
}

TEST(ScanerTest, divide) {
	Scaner scaner = initScaner("/");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::MultOp_);
	auto asPtr = dynamic_cast<MultOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),MultOpType::divide);
}

TEST(ScanerTest, modulo) {
	Scaner scaner = initScaner("%");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::MultOp_);
	auto asPtr = dynamic_cast<MultOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),MultOpType::modulo);
}

TEST(ScanerTest, isEqual) {
	Scaner scaner = initScaner("==");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::RelOp_);
	auto asPtr = dynamic_cast<RelOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),RelOpType::equal);
}

TEST(ScanerTest, notEqual) {
	Scaner scaner = initScaner("!=");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::RelOp_);
	auto asPtr = dynamic_cast<RelOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),RelOpType::not_equal);
}

TEST(ScanerTest, less) {
	Scaner scaner = initScaner("<");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::RelOp_);
	auto asPtr = dynamic_cast<RelOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),RelOpType::less);
}

TEST(ScanerTest, less_equal) {
	Scaner scaner = initScaner("<=");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::RelOp_);
	auto asPtr = dynamic_cast<RelOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),RelOpType::less_equal);
}

TEST(ScanerTest, more) {
	Scaner scaner = initScaner(">");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::RelOp_);
	auto asPtr = dynamic_cast<RelOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),RelOpType::more);
}

TEST(ScanerTest, more_equal) {
	Scaner scaner = initScaner(">=");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::RelOp_);
	auto asPtr = dynamic_cast<RelOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),RelOpType::more_equal);
}

TEST(ScanerTest, blockBegin) {
	Scaner scaner = initScaner("{");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::BlockBegin_);
}

TEST(ScanerTest, blockEnd) {
	Scaner scaner = initScaner("}");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::BlockEnd_);
}

TEST(ScanerTest, Or) {
	Scaner scaner = initScaner("||");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Logic_);
	auto asPtr = dynamic_cast<LogicOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),LogicOpType::or_);
}

TEST(ScanerTest, And) {
	Scaner scaner = initScaner("&&");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Logic_);
	auto asPtr = dynamic_cast<LogicOp*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getSubtype(),LogicOpType::and_);
}

TEST(ScanerTest, String1) {
	Scaner scaner = initScaner("\"k\"");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::String_);
	auto asPtr = dynamic_cast<StringToken*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getValue(),"k");
}

TEST(ScanerTest, String2) {
	Scaner scaner = initScaner(R"("ka&^%#@!^$reugsjg\n\t ")");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::String_);
	auto asPtr = dynamic_cast<StringToken*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getValue(),"ka&^%#@!^$reugsjg\n\t ");
}

TEST(ScanerTest, StringWithSemicolon) {
	Scaner scaner = initScaner("\"k\";");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::String_);
	auto asPtr = dynamic_cast<StringToken*>(tokenPtr.get());
	ASSERT_EQ(asPtr->getValue(),"k");
	tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::End_);

}

TEST(ScanerTest, Number1) {
	Scaner scaner = initScaner("17358");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Number_);
	auto asPtr = dynamic_cast<Number*>(tokenPtr.get());
	ASSERT_TRUE(isIntValueCorrect(*asPtr,17358));
}

TEST(ScanerTest, NumberZero) {
	Scaner scaner = initScaner("0");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Number_);
	auto asPtr = dynamic_cast<Number*>(tokenPtr.get());
	ASSERT_TRUE(isIntValueCorrect(*asPtr,0));
}

TEST(ScanerTest, MinusNumber) {
	Scaner scaner = initScaner("-12");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Minus_);
	tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Number_);
	auto asPtr = dynamic_cast<Number*>(tokenPtr.get());
	ASSERT_TRUE(isIntValueCorrect(*asPtr,12));
}

TEST(ScanerTest, MinusDoubleNumber) {
	Scaner scaner = initScaner("-1.48");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Minus_);
	tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Number_);
	auto asPtr = dynamic_cast<Number*>(tokenPtr.get());
	ASSERT_TRUE(isDoubleValueCorrect(*asPtr,1.48));
}

TEST(ScanerTest, PlusDoubleNumber) {
	Scaner scaner = initScaner("37.12");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Number_);
	auto asPtr = dynamic_cast<Number*>(tokenPtr.get());
	ASSERT_TRUE(isDoubleValueCorrect(*asPtr,37.12));
}

TEST(ScanerTest, Sequence) {
	Scaner scaner = initScaner("int a;");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::TypeName_);
	auto loopPtr = dynamic_cast<TypeName*>(tokenPtr.get());
	ASSERT_EQ(loopPtr->getSubtype(),intType);
	tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Id_);
	auto idTokenPtr = dynamic_cast<IdToken*>(tokenPtr.get());
	ASSERT_EQ(idTokenPtr->getValue(),"a");
	tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::End_);
	tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::EndOfFile);
}

TEST(ScanerTest, ErrorNumber) {
	Scaner scaner = initScaner("07a");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Error_);
	auto errorPtr = dynamic_cast<ErrorToken*>(tokenPtr.get());
	ASSERT_EQ(errorPtr->getExpected(), TokenType::Number_);
}

TEST(ScanerTest, ErrorStringUnfinished) {
	Scaner scaner = initScaner("\"aa");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Error_);
	auto errorPtr = dynamic_cast<ErrorToken*>(tokenPtr.get());
	ASSERT_EQ(errorPtr->getExpected(), TokenType::String_);
	ASSERT_EQ(errorPtr->getErrorType(), ErrorType::unexpectedCharacter);
	ASSERT_EQ(errorPtr->getValue(), "aa\xFF");
}

TEST(ScanerTest, ColumnCheck) {
	Scaner scaner = initScaner(" a");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Id_);
	ASSERT_EQ(tokenPtr->getColumn(), 2);
}

TEST(ScanerTest, LineCheck) {
	Scaner scaner = initScaner("\na");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_NE(tokenPtr,nullptr);
	ASSERT_EQ(tokenPtr->getType(), TokenType::Id_);
	ASSERT_EQ(tokenPtr->getLine(), 2);
}

TEST(ErrorHandlerTest, AddErrorsTest) {
	ErrorHandler handler;
	ErrorToken token(1,1,"_",TokenType::Error_);
	handler.addError(ScanerError, token);
	handler.addError(ScanerError, token);
	handler.addError(ScanerError, token);
	ASSERT_EQ(handler.getErrorSize(),3);
}

TEST(ErrorHandlerTest, ShowNoErrorTest) {
	ErrorHandler::clear();
	std::ostringstream str;
	std::ostringstream eStr;
	ErrorHandler::showErrors(str, eStr);
	std::string string = str.str();
	ASSERT_EQ(string,"No errors\n");
	ASSERT_EQ(eStr.str(),"");
}

TEST(ErrorHandlerTest, ShowErrorTest) {
	ErrorHandler::clear();
	ErrorToken token(1,1,"_",TokenType::Error_);
	ErrorHandler::addError(ScanerError, token);
	ASSERT_EQ(ErrorHandler::getErrorSize(),1);
	std::ostringstream str;
	ErrorHandler::showErrors(std::cout,str);
	std::string string = str.str();
	ASSERT_TRUE(string.find("Errors found: 1")!=std::string::npos);
}
//Parser tests
TEST(ParserTest, EmptyString) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, GlobalVariableTestInt) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, TypeType::int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_FALSE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}
TEST(ParserTest, GlobalVariableTestDouble) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("double a,b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, TypeType::double_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 2);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_EQ(result.globalVars[0].vars[1].first, "b");
	ASSERT_FALSE((bool)result.globalVars[0].vars[1].second);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, EmptyFunctionVoid) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	ASSERT_EQ(result.functions[0].returnedType, void_);
	ASSERT_EQ(result.functions[0].parameters.size(), 0);
	ASSERT_EQ(result.functions[0].name,"f");
	ASSERT_EQ(result.functions[0].block.lines.size(),0);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, SemicolonFunctionWithGlobalVar) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a; void f(){;;;}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_FALSE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(result.functions.size(), 1);
	ASSERT_EQ(result.functions[0].returnedType, void_);
	ASSERT_EQ(result.functions[0].parameters.size(), 0);
	ASSERT_EQ(result.functions[0].name,"f");
	ASSERT_EQ(result.functions[0].block.lines.size(),3);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, EmptyFunctionReturningDoubleWithParameter) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(double b){}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	ASSERT_EQ(result.functions[0].returnedType, void_);
	ASSERT_EQ(result.functions[0].parameters.size(), 1);
	ASSERT_EQ(result.functions[0].parameters[0].type, doubleType);
	ASSERT_EQ(result.functions[0].parameters[0].name, "b");
	ASSERT_EQ(result.functions[0].name,"f");
	ASSERT_EQ(result.functions[0].block.lines.size(),0);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, SeveralFunctionsAndVars) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){} int a; int g(){} double b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	EXPECT_EQ(result.globalVars.size(), 2);
	EXPECT_EQ(result.functions.size(), 2);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, GlobalVarWithValue) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=5;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	const auto& v = result.globalVars[0];
	ASSERT_EQ(v.type, int_);
	ASSERT_EQ(v.vars.size(), 1);
	ASSERT_EQ(v.vars[0].first, "a");
	ASSERT_TRUE((bool)v.vars[0].second);
	ASSERT_EQ(typeid(*(v.vars[0].second)), typeid(NumberExpression));
	const auto expr = dynamic_cast<NumberExpression*>(v.vars[0].second.get());
	ASSERT_EQ(expr->value.index(),0); //check if int
	ASSERT_EQ(std::get<0>(expr->value),5);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, ExpressionAddTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=5+b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,add);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1.get());
	ASSERT_EQ(expr1->value.index(),0);
	ASSERT_EQ(std::get<0>(expr1->value),5);

	ASSERT_TRUE((bool)expr->expression2);
	ASSERT_EQ(typeid(*expr->expression2),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr->expression2.get());
	ASSERT_EQ(expr2->value,"b");
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, ExpressionMultTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=5*b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,mult);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1.get());
	ASSERT_EQ(expr1->value.index(),0);
	ASSERT_EQ(std::get<0>(expr1->value),5);
	ASSERT_TRUE((bool)expr->expression2);
	ASSERT_EQ(typeid(*expr->expression2),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr->expression2.get());
	ASSERT_EQ(expr2->value,"b");
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, ExpressionSubTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=5-b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,minus);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1.get());
	ASSERT_EQ(expr1->value.index(),0);
	ASSERT_EQ(std::get<0>(expr1->value),5);

	ASSERT_TRUE((bool)expr->expression2);
	ASSERT_EQ(typeid(*expr->expression2),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr->expression2.get());
	ASSERT_EQ(expr2->value,"b");
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, UnaryMinusTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=-5;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op, minus);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1.get());
	ASSERT_EQ(expr1->value.index(),0);
	ASSERT_EQ(std::get<0>(expr1->value),5);
	ASSERT_FALSE((bool)expr->expression2);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, ConversionIntTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=(int)5.0;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,toIntConversion);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1.get());
	ASSERT_EQ(expr1->value.index(),1);
	ASSERT_EQ(std::get<1>(expr1->value),5.0);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}


TEST(ParserTest, ConversionDoubleTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("double a=(double)5;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, double_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,toDoubleConversion);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1.get());
	ASSERT_EQ(expr1->value.index(),0);
	ASSERT_EQ(std::get<0>(expr1->value),5);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, NegationTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("double a=!5;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, double_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,negation);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1.get());
	ASSERT_EQ(expr1->value.index(),0);
	ASSERT_EQ(std::get<0>(expr1->value),5);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, CompareTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=5==b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,eq);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1.get());
	ASSERT_EQ(expr1->value.index(),0);
	ASSERT_EQ(std::get<0>(expr1->value),5);

	ASSERT_TRUE((bool)expr->expression2);
	ASSERT_EQ(typeid(*expr->expression2),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr->expression2.get());
	ASSERT_EQ(expr2->value,"b");
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, LogicTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=5||b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,Or);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1.get());
	ASSERT_EQ(expr1->value.index(),0);
	ASSERT_EQ(std::get<0>(expr1->value),5);

	ASSERT_TRUE((bool)expr->expression2);
	ASSERT_EQ(typeid(*expr->expression2),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr->expression2.get());
	ASSERT_EQ(expr2->value,"b");
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, ExpressionSequenceTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=d+b+c;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,add);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(Expression));
	const auto expr1 = expr->expression1.get();
	ASSERT_EQ(expr1->op,add);
	ASSERT_TRUE((bool)expr1->expression1);
	ASSERT_EQ(typeid(*expr1->expression1),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr1->expression1.get());
	ASSERT_EQ(expr2->value,"d");

	ASSERT_TRUE((bool)expr1->expression1);
	ASSERT_EQ(typeid(*expr1->expression1),typeid(IdExpression));
	const auto expr3 = dynamic_cast<IdExpression*>(expr1->expression2.get());
	ASSERT_EQ(expr3->value,"b");

	ASSERT_TRUE((bool)expr->expression2);
	ASSERT_EQ(typeid(*expr->expression2),typeid(IdExpression));
	const auto expr4 = dynamic_cast<IdExpression*>(expr->expression2.get());
	ASSERT_EQ(expr4->value,"c");
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, DifferentPriorities1) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=d*b+c;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,add);
	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(Expression));
	const auto expr1 = expr->expression1.get();
	ASSERT_EQ(expr1->op,mult);
	ASSERT_TRUE((bool)expr1->expression1);
	ASSERT_EQ(typeid(*expr1->expression1),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr1->expression1.get());
	ASSERT_EQ(expr2->value,"d");

	ASSERT_TRUE((bool)expr1->expression1);
	ASSERT_EQ(typeid(*expr1->expression1),typeid(IdExpression));
	const auto expr3 = dynamic_cast<IdExpression*>(expr1->expression2.get());
	ASSERT_EQ(expr3->value,"b");

	ASSERT_TRUE((bool)expr->expression2);
	ASSERT_EQ(typeid(*expr->expression2),typeid(IdExpression));
	const auto expr4 = dynamic_cast<IdExpression*>(expr->expression2.get());
	ASSERT_EQ(expr4->value,"c");
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, DifferentPriorities2) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=d+b*c;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type, int_);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first, "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_EQ(expr->op,add);

	ASSERT_TRUE((bool)expr->expression1);
	ASSERT_EQ(typeid(*expr->expression1),typeid(IdExpression));
	const auto expr4 = dynamic_cast<IdExpression*>(expr->expression1.get());
	ASSERT_EQ(expr4->value,"d");

	ASSERT_TRUE((bool)expr->expression2);
	ASSERT_EQ(typeid(*expr->expression2),typeid(Expression));

	const auto expr1 = expr->expression2.get();
	ASSERT_EQ(expr1->op,mult);
	ASSERT_TRUE((bool)expr1->expression1);
	ASSERT_EQ(typeid(*expr1->expression1),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr1->expression1.get());
	ASSERT_EQ(expr2->value,"b");

	ASSERT_TRUE((bool)expr1->expression2);
	ASSERT_EQ(typeid(*expr1->expression2),typeid(IdExpression));
	const auto expr3 = dynamic_cast<IdExpression*>(expr1->expression2.get());
	ASSERT_EQ(expr3->value,"c");
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, FunctionWithInit) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){int a;}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	const auto& f = result.functions[0];
	ASSERT_EQ(f.returnedType, void_);
	ASSERT_EQ(f.parameters.size(), 0);
	ASSERT_EQ(f.name,"f");
	ASSERT_EQ(f.block.lines.size(), 1);
	auto l = f.block.lines[0];
	ASSERT_EQ(typeid(*l),typeid(InitNode));
	auto il = dynamic_cast<InitNode*>(l.get());
	ASSERT_EQ(il->type,int_);
	ASSERT_EQ(il->vars.size(),1);
	auto v = il->vars[0];
	ASSERT_EQ(v.first,"a");
	ASSERT_FALSE(bool(v.second));
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, FunctionWithInitWithValue) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){int a=5;}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	const auto& f = result.functions[0];
	ASSERT_EQ(f.returnedType, void_);
	ASSERT_EQ(f.parameters.size(), 0);
	ASSERT_EQ(f.name,"f");
	ASSERT_EQ(f.block.lines.size(), 1);
	auto l = f.block.lines[0];
	ASSERT_EQ(typeid(*l),typeid(InitNode));
	auto il = dynamic_cast<InitNode*>(l.get());
	ASSERT_EQ(il->type, int_);
	ASSERT_EQ(il->vars.size(),1);
	auto v = il->vars[0];
	ASSERT_EQ(v.first,"a");
	ASSERT_TRUE(bool(v.second));
	ASSERT_EQ(typeid(*v.second), typeid(NumberExpression));
	const auto expr = dynamic_cast<NumberExpression*>(v.second.get());
	ASSERT_EQ(expr->value.index(),0); //check if int
	ASSERT_EQ(std::get<0>(expr->value),5);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, IfTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){if(1){}}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	const auto& f = result.functions[0];
	ASSERT_EQ(f.returnedType, void_);
	ASSERT_EQ(f.parameters.size(), 0);
	ASSERT_EQ(f.name,"f");
	ASSERT_EQ(f.block.lines.size(), 1);
	auto l = f.block.lines[0];
	ASSERT_EQ(typeid(*l),typeid(IfNode));
	auto ifn = dynamic_cast<IfNode*>(l.get());
	ASSERT_TRUE(bool(ifn->condition));
	ASSERT_EQ(ifn->elseStat,std::nullopt);
	ASSERT_EQ(ifn->stat.lines.size(), 0);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, IfTestWithElse) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){if(1){}else int a;}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	const auto& f = result.functions[0];
	ASSERT_EQ(f.returnedType, void_);
	ASSERT_EQ(f.parameters.size(), 0);
	ASSERT_EQ(f.name,"f");
	ASSERT_EQ(f.block.lines.size(), 1);
	auto l = f.block.lines[0];
	ASSERT_NE(l.get(),nullptr);
	ASSERT_EQ(typeid(*l),typeid(IfNode));
	auto ifn = dynamic_cast<IfNode*>(l.get());
	ASSERT_TRUE(bool(ifn->condition));
	ASSERT_NE(ifn->elseStat,std::nullopt);
	ASSERT_EQ(ifn->stat.lines.size(), 0);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, WhileTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){while(0){}}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	const auto& f = result.functions[0];
	ASSERT_EQ(f.returnedType, void_);
	ASSERT_EQ(f.parameters.size(), 0);
	ASSERT_EQ(f.name,"f");
	ASSERT_EQ(f.block.lines.size(), 1);
	auto l = f.block.lines[0];
	ASSERT_EQ(typeid(*l),typeid(WhileNode));
	auto w = dynamic_cast<WhileNode*>(l.get());
	ASSERT_TRUE(bool(w->condition));
	ASSERT_EQ(w->stat.lines.size(), 0);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, FunCallTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){print(5);}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	const auto& f = result.functions[0];
	ASSERT_EQ(f.returnedType, void_);
	ASSERT_EQ(f.parameters.size(), 0);
	ASSERT_EQ(f.name,"f");
	ASSERT_EQ(f.block.lines.size(), 1);
	auto l = f.block.lines[0];
	ASSERT_TRUE(bool(l));
	ASSERT_EQ(typeid(*l),typeid(FunCall));
	auto fc = dynamic_cast<FunCall*>(l.get());
	ASSERT_EQ(fc->name,"print");
	ASSERT_EQ(fc->params.size(),1);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, ReturnTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){return;}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	const auto& f = result.functions[0];
	ASSERT_EQ(f.returnedType, void_);
	ASSERT_EQ(f.parameters.size(), 0);
	ASSERT_EQ(f.name,"f");
	ASSERT_EQ(f.block.lines.size(), 1);
	auto l = f.block.lines[0];
	ASSERT_TRUE(bool(l));
	ASSERT_EQ(typeid(*l),typeid(ReturnNode));
	auto ret = dynamic_cast<ReturnNode*>(l.get());
	ASSERT_FALSE(bool(ret->returnedValue));
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, LoopModTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){continue;}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	const auto& f = result.functions[0];
	ASSERT_EQ(f.returnedType, void_);
	ASSERT_EQ(f.parameters.size(), 0);
	ASSERT_EQ(f.name,"f");
	ASSERT_EQ(f.block.lines.size(), 1);
	auto l = f.block.lines[0];
	ASSERT_TRUE(bool(l));
	ASSERT_EQ(typeid(*l),typeid(LoopModLine));
	auto lm = dynamic_cast<LoopModLine*>(l.get());
	ASSERT_EQ(lm->type,continueType);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, ReturnValueTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int f(){return 5;}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	const auto& f = result.functions[0];
	ASSERT_EQ(f.returnedType, int_);
	ASSERT_EQ(f.parameters.size(), 0);
	ASSERT_EQ(f.name,"f");
	ASSERT_EQ(f.block.lines.size(), 1);
	auto l = f.block.lines[0];
	ASSERT_TRUE(bool(l));
	ASSERT_EQ(typeid(*l),typeid(ReturnNode));
	auto ret = dynamic_cast<ReturnNode*>(l.get());
	ASSERT_TRUE(bool(ret->returnedValue));
	ASSERT_EQ(typeid(*ret->returnedValue),typeid(NumberExpression));
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, WrongToken) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("07a");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_NE(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, MissingBracket) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int f(){");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	EXPECT_EQ(result.globalVars.size(), 0);
	EXPECT_EQ(result.functions.size(), 1);
	ASSERT_NE(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, MissingSemicolon) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a,b");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_NE(ErrorHandler::getErrorSize(),0);
}

TEST(ParserTest, MissingBracket1) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int f({}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 1);
	ASSERT_NE(ErrorHandler::getErrorSize(),0);
}

TEST(MappedSyntaxTreeTest, Good) {
    ErrorHandler::clear();
    auto&& [wasGood, result] = initMappedSyntaxTree("int main(){} int a; int g(){} double b;");
    EXPECT_TRUE(wasGood);
    EXPECT_EQ(result.globalVars.size(), 2);
    EXPECT_EQ(result.functions.size(), 5); //because there are three pre-defined functions: scan, scanf and print
    ASSERT_NE(result.globalVars.find("a"),result.globalVars.end());
    ASSERT_NE(result.globalVars.find("b"),result.globalVars.end());
    ASSERT_NE(result.functions.find("main"),result.functions.end());
    ASSERT_NE(result.functions.find("g"),result.functions.end());
    //pre-defined
    EXPECT_NE(result.functions.find("scan"),result.functions.end());
    EXPECT_NE(result.functions.find("scanf"),result.functions.end());
    EXPECT_NE(result.functions.find("print"),result.functions.end());
    //
    ASSERT_EQ(ErrorHandler::getErrorSize(),0);
}

TEST(MappedSyntaxTreeTest, NoMain) {
    ErrorHandler::clear();
    auto&& [wasGood, result] = initMappedSyntaxTree("int f(){} int a; int g(){} double b;");
    EXPECT_FALSE(wasGood);
    EXPECT_EQ(result.globalVars.size(), 2);
    ASSERT_NE(result.globalVars.find("a"),result.globalVars.end());
    ASSERT_NE(result.globalVars.find("b"),result.globalVars.end());
    ASSERT_NE(result.functions.find("f"),result.functions.end());
    ASSERT_NE(result.functions.find("g"),result.functions.end());
    ASSERT_EQ(result.functions.find("main"),result.functions.end());
    ASSERT_NE(ErrorHandler::getErrorSize(),0);
}

TEST(MappedSyntaxTreeTest, WrongMain) {
    ErrorHandler::clear();
    auto&& [wasGood, result] = initMappedSyntaxTree("double main(){} int a; int g(){} double b;");
    EXPECT_FALSE(wasGood);
    ASSERT_NE(ErrorHandler::getErrorSize(),0);
}

TEST(MappedSyntaxTreeTest, SameFunctionNames) {
    ErrorHandler::clear();
    auto&& [wasGood, result] = initMappedSyntaxTree("int b(){} int a; int b(){} double b;");
    EXPECT_FALSE(wasGood);
    EXPECT_EQ(result.globalVars.size(), 2);
    ASSERT_NE(result.globalVars.find("a"),result.globalVars.end());
    ASSERT_NE(result.globalVars.find("b"),result.globalVars.end());
    ASSERT_NE(result.functions.find("b"),result.functions.end());
    ASSERT_NE(ErrorHandler::getErrorSize(),0);
}

TEST(MappedSyntaxTreeTest, SameVariableName) {
    ErrorHandler::clear();
    auto&& [wasGood, result] = initMappedSyntaxTree("int f(){} int a; int main(){} double a;");
    EXPECT_FALSE(wasGood);
    EXPECT_EQ(result.functions.size(), 5);
    ASSERT_NE(result.globalVars.find("a"),result.globalVars.end());
    ASSERT_NE(result.functions.find("f"),result.functions.end());
    ASSERT_NE(result.functions.find("main"),result.functions.end());
    ASSERT_NE(ErrorHandler::getErrorSize(),0);
}



TEST(MappedSyntaxTreeTest, IllegalFunctionName) {
    ErrorHandler::clear();
    auto&& [wasGood, result] = initMappedSyntaxTree("int print(){} int a; int b(){} double b;");
    EXPECT_FALSE(wasGood);
    EXPECT_EQ(result.globalVars.size(), 2);
    ASSERT_NE(result.globalVars.find("a"),result.globalVars.end());
    ASSERT_NE(result.globalVars.find("b"),result.globalVars.end());
    ASSERT_NE(result.functions.find("b"),result.functions.end());
    ASSERT_NE(ErrorHandler::getErrorSize(),0);
}

TEST(InterpreterTest, ReturnTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){return 5;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 5);
	ASSERT_EQ(out.str(),"");
}

TEST(InterpreterTest, NumberPrintTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){print(0);return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"0");
}

TEST(InterpreterTest, IntAssignTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){int a; a=5; return a;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 5);
	ASSERT_EQ(out.str(),"");
}

TEST(InterpreterTest, IntAddAssignTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){int a=1; a+=5; return a;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 6);
	ASSERT_EQ(out.str(),"");
}

TEST(InterpreterTest, DoubleAssignTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){double a; a=5; return (int)a;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 5);
	ASSERT_EQ(out.str(),"");
}

TEST(InterpreterTest, DoubleMultAssignTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){double a=1.0; a*=5; return (int)a;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 5);
	ASSERT_EQ(out.str(),"");
}


TEST(InterpreterTest, IntInputPrintTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){int b=scan();print(b);return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	std::istringstream in("6");
	Interpreter interpreter(in, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"6");
}

TEST(InterpreterTest, DoubleInputPrintTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){double b=scanf(); print(b);return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	std::istringstream in("6.2");
	Interpreter interpreter(in, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"6.2");
}

TEST(InterpreterTest, DivideOperationTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){double a=5.0; a=a/5; print(a);return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"1");
}

TEST(InterpreterTest, ModuloOperationTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){int a=7; a=a%3; print(a);return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"1");
}

TEST(InterpreterTest, DivideByZeroErrorOperationTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){double a=5.0; a=a/0;return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	ASSERT_THROW({interpreter.visitTree(result);}, std::runtime_error);
}

TEST(InterpreterTest, ConversionTest1) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){int a=7; a=(int)5.0; print(a);return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"5");
}

TEST(InterpreterTest, ConversionTest2) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){double a; a=(double)5; print(a);return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"5");
}

TEST(InterpreterTest, IfTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){\nif(0==2)\nprint(5);\nprint(7);\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"7");
}

TEST(InterpreterTest, ElseTest1) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){if((0==0) && (7==7)) print(7); else print(4); return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"7");
}

TEST(InterpreterTest, ElseTest2) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){if((0==1) && (7==7)) print(7); else print(4); return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"4");
}

TEST(InterpreterTest, WhileTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main(){int i=0; while(i<4) {i+=1; print(i);} return 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"1234");
}

TEST(InterpreterTest, ForTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main()\n{int i;\nfor(i=0; i<4; i+=1)\n{print(i);}\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"0123");
}

TEST(InterpreterTest, BreakTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main()\n{int i;\nfor(i=0; i<4; i+=1)\n{\n{if(i==2) break; print(i);}\n}\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"01");
}

TEST(InterpreterTest, ContinueTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main()\n{int i;\nfor(i=0; i<4; i+=1)\n{\n{if(i==2) continue; print(i);}\n}\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"013");
}

TEST(InterpreterTest, InternalReturnTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main()\n{int i;\nfor(i=0; i<4; i+=1)\n{\n{if(i==2) return 5; print(i);}\n}\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 5);
	ASSERT_EQ(out.str(),"01");
}

TEST(InterpreterTest, GlobalVarTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int a;int main()\n{a=6;\nprint(a);\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"6");
}

TEST(InterpreterTest, UnknownVarTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main()\n{b=6;\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	ASSERT_THROW({(int) interpreter.visitTree(result);},std::runtime_error);
}

TEST(InterpreterTest, ManyFunctionsTest1) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int a() {return 5;} int main()\n{print(a());\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"5");
}

TEST(InterpreterTest, ManyFunctionsTest2) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int a(int b) {return 2*b;} int main()\n{print(a(5));\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"10");
}

TEST(InterpreterTest, UnknownFunctionTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main()\n{print(a(5));\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	ASSERT_THROW({(int) interpreter.visitTree(result);},std::runtime_error);
}

TEST(InterpreterTest, ManyFunctionsTestWrongParameterNumber) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int a(int b) {return 2*b;} int main()\n{print(a());\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	ASSERT_THROW({(int) interpreter.visitTree(result);},std::runtime_error);
}

TEST(InterpreterTest, PrintStringTest) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main()\n{print(\"\\n\");\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"\n");
}

TEST(InterpreterTest, AddStringTest1) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main()\n{print(\"a\"+\"\\n\");\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"a\n");
}

TEST(InterpreterTest, AddStringTest2) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main()\n{print(5+\"\\n\");\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	int retValue;
	try {retValue = (int) interpreter.visitTree(result);}
	catch(...) {FAIL();}
	ASSERT_EQ(retValue, 0);
	ASSERT_EQ(out.str(),"5\n");
}

TEST(InterpreterTest, IllegalStringOperation) {
	ErrorHandler::clear();
	auto&& [wasGood, result] = initMappedSyntaxTree("int main()\n{print(\"a\"-\"\\n\");\nreturn 0;}");
	ASSERT_TRUE(wasGood);
	ASSERT_EQ(ErrorHandler::getErrorSize(),0);
	std::ostringstream out;
	Interpreter interpreter(std::cin, out);
	ASSERT_THROW({(int) interpreter.visitTree(result);},std::runtime_error);
}

int main(int argc, char** argv) {

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}