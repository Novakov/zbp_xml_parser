#include "stdafx.h"

#include <stack>

#include "parser.h"
#include "tokenizer.h"
#include "tokens.h"


using namespace std;


class ValidatingTokenizer : public Tokenizer
{
public:
	template<typename TToken>
	TToken* current()
	{
		auto fetchedToken = Tokenizer::current();
		if (fetchedToken == NULL)
		{
			throw no_token_exception();
		}
		
		auto castedToken = dynamic_cast<TToken*>(fetchedToken);

		if (castedToken == NULL)
		{
			throw invalid_token_exception();
		}

		return castedToken;
	};
};

class ComposingTokenizer 
{
private:
	ValidatingTokenizer tokenizer;
public:
	void processFrom(istream * stream)
	{
		tokenizer.processFrom(stream);
	}

	bool endOfInput()
	{
		return this->endOfInput();
	}

	Token * nextToken();
};

Token * ComposingTokenizer::nextToken()
{
	return nullptr;
}

shared_ptr<Element> Parser::parse(istream * input)
{
	ValidatingTokenizer tokenizer;
	
	stack<shared_ptr<Element>> elements;
	
	tokenizer.processFrom(input);

	tokenizer.nextToken();
	tokenizer.current<TagStartToken>();
	
	tokenizer.nextToken();
	auto name = tokenizer.current<NameToken>();

	tokenizer.nextToken();
	tokenizer.current<TagEndToken>();

	auto element = make_shared<Element>(name->name());

	elements.push(element);

	while (!tokenizer.endOfInput())
	{
		tokenizer.nextToken();
		tokenizer.current<TagStartToken>();
		
		tokenizer.nextToken();
		auto token = tokenizer.current<Token>();

		switch (token->type())
		{
		case TokenType::T_END_TAG_MARK:
			tokenizer.nextToken();
			tokenizer.current<NameToken>();
			
			tokenizer.nextToken();
			tokenizer.current<TagEndToken>();

			elements.pop();
			break;
		case TokenType::T_NAME:
			auto childElement = make_shared<Element>(dynamic_cast<NameToken*>(token)->name());
			elements.top()->addChild(childElement);
			elements.push(childElement);

			tokenizer.nextToken();
			tokenizer.current<TagEndToken>();

			break;
		}
	}	

	return element;
}