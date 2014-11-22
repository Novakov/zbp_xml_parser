#include "stdafx.h"

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

	tokenizer.processFrom(input);

	tokenizer.nextToken<TagStartToken>();
	auto name = tokenizer.nextToken<NameToken>();
	tokenizer.nextToken<TagEndToken>();
	
	// read contents

	tokenizer.nextToken<TagStartToken>();
	tokenizer.nextToken<EndTagMarkToken>();
	tokenizer.nextToken<NameToken>();
	tokenizer.nextToken<TagEndToken>();

	return make_shared<Element>(name->name());
}