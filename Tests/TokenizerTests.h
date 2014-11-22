#include "stdafx.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <vector>
#include <memory>

#include <tokenizer.h>
#include <tokens.h>


using namespace std;

class TokenizerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(TokenizerTest);
	CPPUNIT_TEST(ShouldTokenizeSingleElementWithNoContent);
	CPPUNIT_TEST(ShouldReturnNoTokensForEmptyString);
	CPPUNIT_TEST_SUITE_END();

private:
	vector<Token*> tokens;
	Tokenizer * tokenizer;

	void tokenize(string input)
	{
		stringstream stream(input);

		this->tokenizer->processFrom(&stream);		

		while (!this->tokenizer->endOfInput())
		{			
			auto token = this->tokenizer->nextToken();
			this->tokens.push_back(token);
		}
	}
public:
	void setUp()
	{		
		this->tokenizer = new Tokenizer();
		this->tokens.clear();
	}

	void tearDown()
	{		
		delete this->tokenizer;
	}

	virtual void ShouldTokenizeSingleElementWithNoContent()
	{
		// arrange
		string s = "<element></element>";

		// act
		tokenize(s);

		// assert		
		CPPUNIT_ASSERT_EQUAL(7, (int)this->tokens.size());
	
		CPPUNIT_ASSERT_EQUAL(T_TAG_START, this->tokens.at(0)->type());
		CPPUNIT_ASSERT_EQUAL(T_NAME, this->tokens.at(1)->type());
		CPPUNIT_ASSERT_EQUAL(string("element"), dynamic_cast<NameToken*>(this->tokens.at(1))->name());
		CPPUNIT_ASSERT_EQUAL(T_TAG_END, this->tokens.at(2)->type());
		
		CPPUNIT_ASSERT_EQUAL(T_TAG_START, this->tokens.at(3)->type());
		CPPUNIT_ASSERT_EQUAL(T_END_TAG_MARK, this->tokens.at(4)->type());
		CPPUNIT_ASSERT_EQUAL(T_NAME, this->tokens.at(5)->type());
		CPPUNIT_ASSERT_EQUAL(string("element"), dynamic_cast<NameToken*>(this->tokens.at(5))->name());
		CPPUNIT_ASSERT_EQUAL(T_TAG_END, this->tokens.at(6)->type());
	}

	virtual void ShouldReturnNoTokensForEmptyString()
	{
		// arrange
		string s = "";

		// act
		tokenize(s);

		// assert
		CPPUNIT_ASSERT_EQUAL(0, (int)this->tokens.size());
	}
};

