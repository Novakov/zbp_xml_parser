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
	bool name(char c, string * s)
	{
		*s = "";
		while ('a' <= c && c <= 'z')
		{
			*s += c;
			this->next();
			c = this->peek();
		}

		return s->length() > 0;
	}

	bool openTag(char c, void * dummy)
	{
		if (c == '<')
		{
			this->next();
			return true;
		}

		return false;
	}

	bool closeTag(char c, void * dummy)
	{
		if (c == '>')
		{
			this->next();
			return true;
		}

		return false;
	}

	bool tagCloseMark(char c, void * dummy)
	{
		if (c == '/')
		{
			this->next();
			return true;
		}

		return false;
	}

	bool startTag(char c, string * tagName)
	{
		if (!test(&XmlGrammar::openTag, (void*)nullptr))
			return false;

		if (!test(&XmlGrammar::name, tagName))
			return false;

		if (!test(&XmlGrammar::closeTag, (void*)nullptr))
			return false;

		return true;
	}

	bool endTag(char c, string * tagName)
	{
		if (!test(&XmlGrammar::openTag, (void*)nullptr))
			return false;

		if (!test(&XmlGrammar::tagCloseMark, (void*)nullptr))
			return false;

		if (!test(&XmlGrammar::name, tagName))
			return false;

		if (!test(&XmlGrammar::closeTag, (void*)nullptr))
			return false;

		return true;
	}

	bool element(char c, shared_ptr<Element> * result)
	{
		string tagName;

		if (!test(&XmlGrammar::startTag, &tagName))		
			return false;		

		*result = make_shared<Element>(tagName);

		if (!test(&XmlGrammar::endTag, &tagName))
			return false;

		return true;
	}

protected:
	bool root(char c, shared_ptr<Element> * result) { return this->element(c, result); }
};

std::shared_ptr<Element> Parser::parse(std::istream * input)
{
	shared_ptr<Element> result = nullptr;

	XmlGrammar grammar;

	if (grammar.isValid(*input, &result))
	{		
		return result;
	}

	return nullptr;
}