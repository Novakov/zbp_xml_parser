#include <istream>
#include <memory>

class Token;

class TokenListener;

class Tokenizer
{
private:
	TokenListener *listener;

	bool isNameChar(char c);
public:
	Tokenizer(TokenListener *listener) : listener(listener)
	{}

	void process(std::istream &input);
};



class TokenListener
{
public:
	virtual void handle(std::shared_ptr<Token> token) = 0;
};