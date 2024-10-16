#pragma once
#include <string>
#include "Util.h"

enum TokenType: uint16_t {
	Id_=0u,
	If_=1u,
	Else_=2u,
	While_=3u,
	For_=4u,
	LoopMod_=5u,
	TypeName_=6u,
	ParBegin_=7u,
	ParEnd_=8u,
	End_=9u,
	Comma_=10u,
	NegOp_=11u,
	Minus_=12u,
	Add_=13u,
	Return_=14u,
	Assign_=15u,
	MultOp_=16u,
	RelOp_=17u,
	BlockBegin_=18u,
	BlockEnd_=19u,
	Logic_=20u,
	Conversion_=21u,
	String_=22u,
	Number_=23u,
	Error_=24u,
	Void_=25u,
	EndOfFile = UINT16_MAX
};
class Token {
protected:
	int line;
	int column;
	TokenType type;
public:
	Token(TokenType type, int line, int column);
	Token()=default;
	TokenType getType() const;
	int getLine() const;
	int getColumn() const;
	virtual ~Token()=default;
};
