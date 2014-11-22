#include "stdafx.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <vector>
#include <memory>

#include <tokenizer.h>
#include <tokens.h>


using namespace std;

class BufferingTokenListener : public TokenListener
{
private:
	vector<shared_ptr<Token>> _tokens;
public:
	void handle(shared_ptr<Token> token)
	{
		this->_tokens.push_back(token);
	}

	vector<shared_ptr<Token>>& tokens()
	{
		return this->_tokens;
	}
};

class TokenizerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(TokenizerTest);
	CPPUNIT_TEST(ShouldTokenizeSingleElementWithNoContent);
	CPPUNIT_TEST_SUITE_END();

private:
	Tokenizer * tokenizer;
	BufferingTokenListener * tokenBuffer;

	void tokenize(string input)
	{
		stringstream stream(input);

		this->tokenizer->process(stream);
	}
public:
	void setUp()
	{
		this->tokenBuffer = new BufferingTokenListener();
		this->tokenizer = new Tokenizer(this->tokenBuffer);
	}

	void tearDown()
	{
		delete this->tokenBuffer;
		delete this->tokenizer;
	}

	virtual void ShouldTokenizeSingleElementWithNoContent()
	{
		// arrange
		string s = "<element></element>";

		// act
		tokenize(s);

		// assert		
		CPPUNIT_ASSERT_EQUAL(7, (int)this->tokenBuffer->tokens().size());
	
		CPPUNIT_ASSERT_EQUAL(T_TAG_START, this->tokenBuffer->tokens().at(0)->type());
		CPPUNIT_ASSERT_EQUAL(T_NAME, this->tokenBuffer->tokens().at(1)->type());
		CPPUNIT_ASSERT_EQUAL(string("element"), dynamic_cast<NameToken*>(this->tokenBuffer->tokens().at(1).get())->name());
		CPPUNIT_ASSERT_EQUAL(T_TAG_END, this->tokenBuffer->tokens().at(2)->type());
		
		CPPUNIT_ASSERT_EQUAL(T_TAG_START, this->tokenBuffer->tokens().at(3)->type());
		CPPUNIT_ASSERT_EQUAL(T_END_TAG_MARK, this->tokenBuffer->tokens().at(4)->type());
		CPPUNIT_ASSERT_EQUAL(T_NAME, this->tokenBuffer->tokens().at(5)->type());
		CPPUNIT_ASSERT_EQUAL(string("element"), dynamic_cast<NameToken*>(this->tokenBuffer->tokens().at(5).get())->name());
		CPPUNIT_ASSERT_EQUAL(T_TAG_END, this->tokenBuffer->tokens().at(6)->type());
	}
};

