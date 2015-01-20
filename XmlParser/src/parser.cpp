#include "stdafx.h"

#include "parser.h"

using namespace std;

class Tokenizer
{
private:
	istream& input;
public:
	Tokenizer(istream& input) : input(input){}

	char peek() { return this->input.peek(); }
	void next() { this->input.get(); }

	int position() { return this->input.tellg(); }
	void rewind(int position) { this->input.seekg(position); }

	bool eof()
	{
		char c = this->input.peek();
		return c == EOF;
	}
};

template<class TGrammar, class TRootResult>
class Grammar
{
private:
	Tokenizer * tokenizer;

protected:
	char peek() { return this->tokenizer->peek(); }
	void next() { this->tokenizer->next(); }

	virtual bool root(char c, TRootResult * result) = 0;
	template<class TResult>
	bool test(bool (TGrammar::*function)(char c, TResult * result), TResult * result)
	{
		auto pos = this->tokenizer->position();

		auto gram = (TGrammar*)this;

		auto hasMatch = (gram->*function)(this->peek(), result);

		if (!hasMatch)
		{
			this->tokenizer->rewind(pos);
		}

		return hasMatch;
	}

	bool parseChar(char c)
	{
		if (this->peek() == c)
		{
			this->next();
			return true;
		}
		return false;
	}

	bool skipWhiteSpace(bool required = false)
	{
		char c = this->peek();

		while (c == ' ' || c == '\n')
		{
			required = false;

			this->next();
			c = this->peek();
		}

		return !required;
	}

public:
	bool Grammar::isValid(istream &input, TRootResult * result)
	{
		this->tokenizer = new Tokenizer(input);

		auto c = this->tokenizer->peek();

		if (this->test(&Grammar::root, result))
		{
			return this->tokenizer->eof();
		}

		return false;
	}
};

class XmlGrammar : public Grammar < XmlGrammar, shared_ptr<Element> >
{
private:
	bool isNameChar(char c)
	{		
		return c != '\n' && c != '\r' && c != '\t' && c != ' '
			&& c != '<' && c != '>' && c != '=' 
			&& c != '/'
			&& c != EOF;
	}

	bool name(char c, string * s)
	{
		*s = "";
		while (isNameChar(c))
		{
			*s += c;
			this->next();
			c = this->peek();
		}

		return s->length() > 0;
	}

	bool attributeValue(char c, string * result)
	{
		*result = "";

		while (c != '"')
		{
			*result += c;

			this->next();

			c = this->peek();
		}

		return true;
	}

	bool attribute(char c, pair<string, string> * result)
	{
		string attributeName;
		string value;

		if (!test(&XmlGrammar::name, &attributeName))
			return false;

		if (!parseChar('='))
			return false;

		if (!parseChar('"'))
			return false;

		if (!test(&XmlGrammar::attributeValue, &value))
			return false;

		if (!parseChar('"'))
			return false;

		*result = make_pair(attributeName, value);

		return true;
	}

	bool startTag(char c, shared_ptr<Element> * element)
	{
		if (!parseChar('<'))
			return false;

		string tagName = "";

		if (!test(&XmlGrammar::name, &tagName))
			return false;

		*element = make_shared<Element>(tagName);

		skipWhiteSpace();

		pair<string, string> attribute;

		while (test(&XmlGrammar::attribute, &attribute))
		{
			(*element)->setAttribute(attribute.first, attribute.second);

			if (!skipWhiteSpace(true))
				break;
		}

		if (!parseChar('>'))
			return false;

		return true;
	}

	bool endTag(char c, string * tagName)
	{
		if (!parseChar('<'))
			return false;

		if (!parseChar('/'))
			return false;

		if (!test(&XmlGrammar::name, tagName))
			return false;

		if (!parseChar('>'))
			return false;

		return true;
	}

	bool text(char c, shared_ptr<Element> * parent)
	{
		bool any = false;

		while (c != '<')
		{
			any = true;
			(*parent)->content += c;

			this->next();
			c = this->peek();
		}

		return any;
	}

	bool elementContent(char c, shared_ptr<Element> * parent)
	{
		shared_ptr<Element> child = nullptr;
		if (test(&XmlGrammar::element, &child))
		{
			(*parent)->addChild(child);
			return true;
		}

		if (test(&XmlGrammar::text, parent))
		{
			return true;
		}

		return false;
	}

	bool fullElement(char c, shared_ptr<Element> * result)
	{
		string tagName;

		if (!test(&XmlGrammar::startTag, result))		
			return false;			
		
		while (test(&XmlGrammar::elementContent, result))
		{
			
		}

		if (!test(&XmlGrammar::endTag, &tagName))
			return false;

		return true;
	}

	bool selfClosedElement(char c, shared_ptr<Element> * result)
	{
		if (!parseChar('<')) 
			return false;

		string tagName = "";

		if (!test(&XmlGrammar::name, &tagName))
			return false;

		*result = make_shared<Element>(tagName);

		skipWhiteSpace();

		pair<string, string> attribute;

		while (test(&XmlGrammar::attribute, &attribute))
		{
			(*result)->setAttribute(attribute.first, attribute.second);

			if (!skipWhiteSpace(true))
				break;
		}

		if (!parseChar('/'))
			return false;
		if (!parseChar('>'))
			return false;
	}

	bool element(char c, shared_ptr<Element> * result)
	{
		shared_ptr<Element> el = nullptr;
		if (test(&XmlGrammar::fullElement, &el))
		{
			*result = el;

			skipWhiteSpace();

			return true;
		}

		if (test(&XmlGrammar::selfClosedElement, &el))
		{
			*result = el;

			skipWhiteSpace();

			return true;
		}

		return false;
	}

protected:
	bool root(char c, shared_ptr<Element> * result) { return this->element(c, result); }
};

std::shared_ptr<Element> Parser::parse(std::istream& input)
{
	shared_ptr<Element> result = nullptr;

	XmlGrammar grammar;

	if (grammar.isValid(input, &result))
	{		
		return result;
	}

	return nullptr;
}