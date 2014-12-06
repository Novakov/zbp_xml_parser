#include "stdafx.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <vector>
#include <memory>

#include <parser.h>

using namespace std;

class ParserTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ParserTest);
	CPPUNIT_TEST(ShouldParseSingleElementWithNoContent);
	CPPUNIT_TEST(ShouldParseSingleElementWithOneNestedElement);
	CPPUNIT_TEST(ShouldParseSingleElementWithTwoNestedElements);
	CPPUNIT_TEST(ShouldParseThreeLevelStructure);
	CPPUNIT_TEST(ShouldParseSelfClosingElement);
	CPPUNIT_TEST(ShouldParseElementsWithMixedFullAndSelfClosedElements);
	CPPUNIT_TEST(ShouldParseElementWithOneAttribute);
	CPPUNIT_TEST(ShouldParseElementWithThreeAttributes);
	CPPUNIT_TEST(ShouldParseSelfClosingElementWithOneAttribute);
	CPPUNIT_TEST(ShouldParseSelfClosingElementWithThreeAttributes);
	CPPUNIT_TEST(ShouldParseElementWithOnlyTextContent);
	CPPUNIT_TEST(ShouldConcatAllTextInsideElement);
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

	virtual void ShouldParseSelfClosingElement()
	{
		// arrange
		string input = "<root/>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"root", result->name());
	}

	virtual void ShouldParseElementsWithMixedFullAndSelfClosedElements()
	{
		// arrange
		string input = "<root><a></a><b/><c><d/></c></root>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"root", result->name());
		CPPUNIT_ASSERT_EQUAL(3, result->childrenCount());
		CPPUNIT_ASSERT_EQUAL((string)"a", result->child(0)->name());
		CPPUNIT_ASSERT_EQUAL((string)"b", result->child(1)->name());
		CPPUNIT_ASSERT_EQUAL((string)"c", result->child(2)->name());
		CPPUNIT_ASSERT_EQUAL(1, result->child(2)->childrenCount());
		CPPUNIT_ASSERT_EQUAL((string)"d", result->child(2)->child(0)->name());
	}

	virtual void ShouldParseElementWithOneAttribute()
	{
		// arrange
		string input = "<root attribute=\"value with spaces\"></root>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"root", result->name());
		CPPUNIT_ASSERT_EQUAL(1, result->attributeCount());
		CPPUNIT_ASSERT_EQUAL((string)"value with spaces", result->attribute("attribute"));
	}

	virtual void ShouldParseElementWithThreeAttributes()
	{
		// arrange
		string input = "<root attr1=\"value 1\" attr2=\"value 2\" attr3=\"value 3\"></root>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"root", result->name());
		CPPUNIT_ASSERT_EQUAL(3, result->attributeCount());
		CPPUNIT_ASSERT_EQUAL((string)"value 1", result->attribute("attr1"));
		CPPUNIT_ASSERT_EQUAL((string)"value 2", result->attribute("attr2"));
		CPPUNIT_ASSERT_EQUAL((string)"value 3", result->attribute("attr3"));
	}

	virtual void ShouldParseSelfClosingElementWithOneAttribute()
	{
		// arrange
		string input = "<root attribute=\"value with spaces\"/>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"root", result->name());
		CPPUNIT_ASSERT_EQUAL(1, result->attributeCount());
		CPPUNIT_ASSERT_EQUAL((string)"value with spaces", result->attribute("attribute"));
	}

	virtual void ShouldParseSelfClosingElementWithThreeAttributes()
	{
		// arrange
		string input = "<root attr1=\"value 1\" attr2=\"value 2\" attr3=\"value 3\" />";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"root", result->name());
		CPPUNIT_ASSERT_EQUAL(3, result->attributeCount());
		CPPUNIT_ASSERT_EQUAL((string)"value 1", result->attribute("attr1"));
		CPPUNIT_ASSERT_EQUAL((string)"value 2", result->attribute("attr2"));
		CPPUNIT_ASSERT_EQUAL((string)"value 3", result->attribute("attr3"));
	}

	virtual void ShouldParseElementWithOnlyTextContent()
	{
		// arrange
		string input = "<root>some text in here</root>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"some text in here", result->content);
	}

	virtual void ShouldConcatAllTextInsideElement()
	{
		// arrange
		string input = "<root>1234<child></child>5678<a></a>xxxx</root>";

		// act
		auto result = parse(input);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
		CPPUNIT_ASSERT_EQUAL((string)"12345678xxxx", result->content);
		CPPUNIT_ASSERT_EQUAL(2, result->childrenCount());
	}
};