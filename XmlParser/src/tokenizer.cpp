#include "stdafx.h"

#include "tokenizer.h"
#include "tokens.h"

using namespace std;

void Tokenizer::processFrom(istream * input)
{
	this->input = input;
}

bool Tokenizer::endOfInput()
{
	this->input->peek();
	return this->input->eof();
}

Token * Tokenizer::nextToken()
{	
	char c;
	*this->input >> c;

	switch (c)
	{
	case '<':
		return new TagStartToken();
		break;
	case '>':
		return new TagEndToken();			
	case '/':
		return new EndTagMarkToken();		
	default:
		if (this->isNameChar(c))
		{
			string name;

			name += c;

			while (!this->input->eof() && this->isNameChar(this->input->peek()))
			{
				*this->input >> c;
				name += c;				
			}

			return new NameToken(name);
		}
	}
}



bool Tokenizer::isNameChar(char c)
{
	if (c == '<') return false;
	if (c == '>') return false;
	if (c == '/') return false;
	if (c == ' ') return false;
	if (c == '\t') return false;
	if (c == '\n') return false;
	if (c == '\r') return false;

	return true;
}