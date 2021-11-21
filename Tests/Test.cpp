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
	ASSERT_EQ(errorPtr->getValue(), "\"aa\xFF");
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}