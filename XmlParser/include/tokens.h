#include <string>

enum TokenType
{
	T_TAG_START, T_NAME, T_TAG_END, T_END_TAG_MARK
};

class Token
{
public:
	virtual TokenType type() = 0;
};

template<TokenType TType>
class BaseToken : public Token
{
public:
	TokenType type() { return TType; }
};

class TagStartToken : public BaseToken<T_TAG_START>
{};

class TagEndToken : public BaseToken<T_TAG_END>
{};

class EndTagMarkToken : public BaseToken<T_END_TAG_MARK>
{};

class NameToken : public BaseToken<T_NAME>
{
private:
	std::string _name;
public:
	NameToken(const std::string name) :_name(name){}

	const std::string name() { return this->_name; }
};
