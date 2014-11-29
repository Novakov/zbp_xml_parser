#include <istream>
#include <memory>

#pragma once

class Token;

class Tokenizer
{
private:
	std::istream * input;
	Token * currentToken;

	bool isNameChar(char c);
	Token * decodeToken();
public:
	void processFrom(std::istream * input);

	bool endOfInput();

	Token * current() { return this->currentToken; };

	bool nextToken();
};
