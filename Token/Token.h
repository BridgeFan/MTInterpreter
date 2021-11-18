#pragma once
#include <string>

enum TokenType: uint16_t {
	Id_,
	If_,
	Else_,
	While_,
	For_,
	LoopMod_,
	TypeName_,
	ParBegin_,
	ParEnd_,
	End_,
	Comma_,
	NegOp_,
	Minus_,
	Add_,
	Return_,
	Assign_,
	MultOp_,
	RelOp_,
	BlockBegin_,
	BlockEnd_,
	Logic_,
	Conversion_,
	String_,
	Number_,
	Error_,
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

