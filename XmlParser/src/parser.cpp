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
	TToken* nextToken()
	{
		auto fetchedToken = Tokenizer::nextToken();
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

shared_ptr<Element> Parser::parse(istream * input)
{
	ValidatingTokenizer tokenizer;
	
	stack<shared_ptr<Element>> elements;
	
	tokenizer.processFrom(input);

	tokenizer.nextToken<TagStartToken>();
	auto name = tokenizer.nextToken<NameToken>();
	tokenizer.nextToken<TagEndToken>();

	auto element = make_shared<Element>(name->name());

	elements.push(element);

	while (!tokenizer.endOfInput())
	{
		tokenizer.nextToken<TagStartToken>();
		auto token = tokenizer.nextToken<Token>();

		switch (token->type())
		{
		case TokenType::T_END_TAG_MARK:
			tokenizer.nextToken<NameToken>();
			tokenizer.nextToken<TagEndToken>();
			elements.pop();
			break;
		case TokenType::T_NAME:
			auto childElement = make_shared<Element>(dynamic_cast<NameToken*>(token)->name());
			elements.top()->addChild(childElement);
			elements.push(childElement);

			tokenizer.nextToken<TagEndToken>();

			break;
		}
	}	

	return element;
}