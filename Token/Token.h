#pragma once
#include <string>

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
};
class Token {
protected:
	int line;
	int column;
	TokenType type;
public:
	static constexpr int maxLength = 128; //maximum length of word
	static bool isLetter(char a);
	static bool isDigit(char a);
	static bool isWhite(char a);
	static bool isDefined(char a);
	static bool isStringCompatible(char a);
	Token(int line, int column);
	TokenType getType() const;
	const std::string& getRaw() const;
	int getLine() const;
	int getColumn() const;
	virtual bool addChar(char c)=0;
	virtual void finish();
	virtual ~Token()=default;
};

class If: public Token {
public:
	If(int line, int column);
	bool addChar(char c) override;
};

class Else: public Token {
public:
	Else(int line, int column);
	bool addChar(char c) override;
};

class While: public Token {
public:
	While(int line, int column);
	bool addChar(char c) override;
};

class For: public Token {
public:
	For(int line, int column);
	bool addChar(char c) override;
};

class ParBegin: public Token {
public:
	ParBegin(int line, int column);
	bool addChar(char c) override;
};

class ParEnd: public Token {
public:
	ParEnd(int line, int column);
	bool addChar(char c) override;
};

class End: public Token {
public:
	End(int line, int column);
	bool addChar(char c) override;
};

class Comma: public Token {
public:
	Comma(int line, int column);
	bool addChar(char c) override;
};

class NegOp: public Token {
public:
	NegOp(int line, int column);
	bool addChar(char c) override;
};

class Minus: public Token {
public:
	Minus(int line, int column);
	bool addChar(char c) override;
};

class Add: public Token {
public:
	Add(int line, int column);
	bool addChar(char c) override;
};

class BlockBegin: public Token {
public:
	BlockBegin(int line, int column);
	bool addChar(char c) override;
};

class BlockEnd: public Token {
public:
	BlockEnd(int line, int column);
	bool addChar(char c) override;
};

class Return: public Token {
public:
	Return(int line, int column);
	bool addChar(char c) override;
};

