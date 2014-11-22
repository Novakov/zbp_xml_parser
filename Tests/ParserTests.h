#include "stdafx.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <vector>
#include <memory>

#include <parser.h>
#include <tokens.h>

using namespace std;

class ParserTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ParserTest);	
	CPPUNIT_TEST(ShouldParseSingleElementWithNoContent);
	CPPUNIT_TEST_SUITE_END();

private:
	Parser * parser;

	shared_ptr<Element> parse(string input)
	{
		stringstream stream(input);

		return this->parser->parse(&stream);
	}
public:
	void setUp()
	{
		this->parser = new Parser();
	}

	void tearDown()
	{
		delete this->parser;
	}

	virtual void ShouldParseSingleElementWithNoContent()
	{
		// arrange
		string input = "<element></element>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT_EQUAL((string)"element", result->name());
	}
};