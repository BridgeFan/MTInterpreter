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
#include "../ErrorHandler.h"
#include "../Parser.h"

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
	ASSERT_EQ(tokenPtr.get(),nullptr);
}

TEST(ScanerTest, White) {
	Scaner scaner = initScaner("   \t\t\n");
	auto tokenPtr = scaner.getNextToken();
	ASSERT_EQ(tokenPtr.get(),nullptr);
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
	ASSERT_EQ(tokenPtr,nullptr);
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
}

TEST(ParserTest, GlobalVariableTestInt) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type.getSubtype(), TypeNameType::intType);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first.getValue(), "a");
	ASSERT_FALSE((bool)result.globalVars[0].vars[0].second);
}
TEST(ParserTest, GlobalVariableTestDouble) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("double a,b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type.getSubtype(), TypeNameType::doubleType);
	ASSERT_EQ(result.globalVars[0].vars.size(), 2);
	ASSERT_EQ(result.globalVars[0].vars[0].first.getValue(), "a");
	ASSERT_EQ(result.globalVars[0].vars[1].first.getValue(), "b");
	ASSERT_FALSE((bool)result.globalVars[0].vars[1].second);
}

TEST(ParserTest, EmptyFunctionVoid) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	ASSERT_EQ(result.functions[0].returnedType.index(), 1);
	ASSERT_EQ(std::get<1>(result.functions[0].returnedType).getType(), Void_);
	ASSERT_EQ(result.functions[0].parameters.size(), 0);
	ASSERT_EQ(result.functions[0].id.getValue(),"f");
	ASSERT_EQ(result.functions[0].block.lines.size(),0);
}

TEST(ParserTest, SemicolonFunctionReturningIntWithGlobalVar) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a; void f(){;}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type.getSubtype(), TypeNameType::intType);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first.getValue(), "a");
	ASSERT_FALSE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(result.functions.size(), 1);
	ASSERT_EQ(result.functions[0].returnedType.index(), 1);
	ASSERT_EQ(std::get<1>(result.functions[0].returnedType).getType(), Void_);
	ASSERT_EQ(result.functions[0].parameters.size(), 0);
	ASSERT_EQ(result.functions[0].id.getValue(),"f");
	ASSERT_EQ(result.functions[0].block.lines.size(),0); //because meaningless semicolons are ignored
}

TEST(ParserTest, EmptyFunctionReturningDoubleWithParameter) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(double b){}");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 0);
	ASSERT_EQ(result.functions.size(), 1);
	ASSERT_EQ(result.functions[0].returnedType.index(), 1);
	ASSERT_EQ(std::get<1>(result.functions[0].returnedType).getType(), Void_);
	ASSERT_EQ(result.functions[0].parameters.size(), 1);
	ASSERT_EQ(result.functions[0].parameters[0].type.getSubtype(), doubleType);
	ASSERT_EQ(result.functions[0].parameters[0].name.getValue(), "b");
	ASSERT_EQ(result.functions[0].id.getValue(),"f");
	ASSERT_EQ(result.functions[0].block.lines.size(),0);
}

TEST(ParserTest, SeveralFunctionsAndVars) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("void f(){} int a; int g(){} double b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.globalVars.size(), 2);
	ASSERT_EQ(result.functions.size(), 2);
}

TEST(ParserTest, GlobalVarWithValue) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=5;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type.getSubtype(), TypeNameType::intType);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first.getValue(), "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(NumberExpression));
	const auto expr = dynamic_cast<NumberExpression*>(result.globalVars[0].vars[0].second.get());
	ASSERT_EQ(expr->token.getValue().index(),0); //check if int
	ASSERT_EQ(std::get<0>(expr->token.getValue()),5);
}

TEST(ParserTest, ExpressionAddTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=5+b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type.getSubtype(), TypeNameType::intType);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first.getValue(), "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_NE(expr->op,std::nullopt);
	ASSERT_EQ((*expr->op)->getType(),Add_);
	ASSERT_NE(expr->expression1,std::nullopt);
	ASSERT_EQ(typeid(**expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1->get());
	ASSERT_EQ(expr1->token.getValue().index(),0);
	ASSERT_EQ(std::get<0>(expr1->token.getValue()),5);

	ASSERT_NE(expr->expression2,std::nullopt);
	ASSERT_EQ(typeid(**expr->expression2),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr->expression2->get());
	ASSERT_EQ(expr2->token.getValue(),"b");
}

TEST(ParserTest, ExpressionMultTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=5*b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type.getSubtype(), TypeNameType::intType);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first.getValue(), "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_NE(expr->op,std::nullopt);
	ASSERT_EQ((*expr->op)->getType(),MultOp_);
	ASSERT_NE(expr->expression1,std::nullopt);
	ASSERT_EQ(typeid(**expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1->get());
	ASSERT_EQ(expr1->token.getValue().index(),0);
	ASSERT_EQ(std::get<0>(expr1->token.getValue()),5);

	ASSERT_NE(expr->expression2,std::nullopt);
	ASSERT_EQ(typeid(**expr->expression2),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr->expression2->get());
	ASSERT_EQ(expr2->token.getValue(),"b");
}

TEST(ParserTest, ExpressionSubTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=5-b;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type.getSubtype(), TypeNameType::intType);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first.getValue(), "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_NE(expr->op,std::nullopt);
	ASSERT_EQ((*expr->op)->getType(),Minus_);
	ASSERT_NE(expr->expression1,std::nullopt);
	ASSERT_EQ(typeid(**expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1->get());
	ASSERT_EQ(expr1->token.getValue().index(),0);
	ASSERT_EQ(std::get<0>(expr1->token.getValue()),5);

	ASSERT_NE(expr->expression2,std::nullopt);
	ASSERT_EQ(typeid(**expr->expression2),typeid(IdExpression));
	const auto expr2 = dynamic_cast<IdExpression*>(expr->expression2->get());
	ASSERT_EQ(expr2->token.getValue(),"b");
}

TEST(ParserTest, UnaryMinusTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=-5;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type.getSubtype(), TypeNameType::intType);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first.getValue(), "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_NE(expr->op,std::nullopt);
	ASSERT_EQ((*expr->op)->getType(),Minus_);
	ASSERT_NE(expr->expression1,std::nullopt);
	ASSERT_EQ(typeid(**expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1->get());
	ASSERT_EQ(expr1->token.getValue().index(),0);
	ASSERT_EQ(std::get<0>(expr1->token.getValue()),5);

	ASSERT_EQ(expr->expression2,std::nullopt);
}

TEST(ParserTest, ConversionIntTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("int a=(int)5.0;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type.getSubtype(), TypeNameType::intType);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first.getValue(), "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_NE(expr->op,std::nullopt);
	ASSERT_EQ((*expr->op)->getType(),Conversion_);
	ASSERT_EQ(dynamic_cast<Conversion*>(expr->op->get())->getSubtype(),toInt);
	ASSERT_NE(expr->expression1,std::nullopt);
	ASSERT_EQ(typeid(**expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1->get());
	ASSERT_EQ(expr1->token.getValue().index(),1);
	ASSERT_EQ(std::get<1>(expr1->token.getValue()),5.0);
}


TEST(ParserTest, ConversionDoubleTest) {
	ErrorHandler::clear();
	Scaner scaner = initScaner("double a=(double)5;");
	Parser parser(scaner);
	SyntaxTree result = parser.parse();
	ASSERT_EQ(result.functions.size(), 0);
	ASSERT_EQ(result.globalVars.size(), 1);
	ASSERT_EQ(result.globalVars[0].type.getSubtype(), TypeNameType::doubleType);
	ASSERT_EQ(result.globalVars[0].vars.size(), 1);
	ASSERT_EQ(result.globalVars[0].vars[0].first.getValue(), "a");
	ASSERT_TRUE((bool)result.globalVars[0].vars[0].second);
	ASSERT_EQ(typeid(*result.globalVars[0].vars[0].second), typeid(Expression));
	const auto expr = result.globalVars[0].vars[0].second.get();
	ASSERT_NE(expr->op,std::nullopt);
	ASSERT_EQ((*expr->op)->getType(),Conversion_);
	ASSERT_EQ(dynamic_cast<Conversion*>(expr->op->get())->getSubtype(),toDouble);
	ASSERT_NE(expr->expression1,std::nullopt);
	ASSERT_EQ(typeid(**expr->expression1),typeid(NumberExpression));
	const auto expr1 = dynamic_cast<NumberExpression*>(expr->expression1->get());
	ASSERT_EQ(expr1->token.getValue().index(),0);
	ASSERT_EQ(std::get<0>(expr1->token.getValue()),5);
}



int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}