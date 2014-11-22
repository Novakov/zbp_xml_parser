#include <istream>
#include <memory>

class Token;

class Tokenizer
{
private:
	std::istream * input;

	bool isNameChar(char c);
public:
	void processFrom(std::istream * input);

	bool endOfInput();

	Token * nextToken();
};
