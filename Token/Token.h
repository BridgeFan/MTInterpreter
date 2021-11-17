#pragma once
#include <string>

enum TokenType: uint16_t {
	Id,

};
class Token {
protected:
	int line;
	int column;
	std::string raw;
	TokenType type;
public:
	Token(int line, int column, std::string raw);
	[[nodiscard]] TokenType getType() const;
	virtual void addChar(char c)=0;
	virtual void finish();
	virtual ~Token()=default;
};
