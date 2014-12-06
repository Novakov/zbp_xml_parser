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
	CPPUNIT_TEST(ShouldParseSingleElementWithOneNestedElement);
	CPPUNIT_TEST(ShouldParseSingleElementWithTwoNestedElements);
	CPPUNIT_TEST(ShouldParseThreeLevelStructure);
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
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"element", result->name());
	}
	
	virtual void ShouldParseSingleElementWithOneNestedElement()
	{
		// arrange
		string input = "<root><a></a></root>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"root", result->name());
		CPPUNIT_ASSERT_EQUAL(1, result->childrenCount());
		CPPUNIT_ASSERT_EQUAL((string)"a", result->child(0)->name());		
	}

	virtual void ShouldParseSingleElementWithTwoNestedElements()
	{
		// arrange
		string input = "<root><a></a><b></b></root>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"root", result->name());
		CPPUNIT_ASSERT_EQUAL(2, result->childrenCount());
		CPPUNIT_ASSERT_EQUAL((string)"a", result->child(0)->name());
		CPPUNIT_ASSERT_EQUAL((string)"b", result->child(1)->name());
	}

	virtual void ShouldParseThreeLevelStructure()
	{
		// arrange
		string input = "<root><a></a><b><c></c></b><d></d></root>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"root", result->name());
		CPPUNIT_ASSERT_EQUAL(3, result->childrenCount());
		CPPUNIT_ASSERT_EQUAL((string)"a", result->child(0)->name());
		CPPUNIT_ASSERT_EQUAL((string)"b", result->child(1)->name());
		CPPUNIT_ASSERT_EQUAL((string)"d", result->child(2)->name());
		CPPUNIT_ASSERT_EQUAL((string)"c", result->child(1)->child(0)->name());
	}
};