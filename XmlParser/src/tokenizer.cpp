#include "stdafx.h"

#include "tokenizer.h"
#include "tokens.h"

using namespace std;

void Tokenizer::process(istream &input)
{
	char c;
	input >> c;

	while (!input.eof())
	{				
		switch (c)
		{
		case '<':			
			this->listener->handle(make_shared<TagStartToken>());
			input >> c;
			break;
		case '>':
			this->listener->handle(make_shared<TagEndToken>());
			input >> c;
			break;
		case '/':
			this->listener->handle(make_shared<EndTagMarkToken>());
			input >> c;
			break;
		default:
			if (this->isNameChar(c))
			{
				string name;

				while (this->isNameChar(c))
				{
					name += c;

					input >> c;
				}

				this->listener->handle(make_shared<NameToken>(name));
			}
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