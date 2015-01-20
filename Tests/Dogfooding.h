#include "stdafx.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <vector>
#include <memory>

#include <dom.h>
#include <parser.h>

using namespace std;

class Dogfooding : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Dogfooding);
	CPPUNIT_TEST(WriteAndParse);	
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void WriteAndParse()
	{
		// arrange
		auto root = Element::create("root");
		root->setAttribute("id", "2");
		root->setAttribute("name", "The name");

		for (int x = 0; x < 3; x++)
		{
			auto childX = Element::create("child");
			root->addChild(childX);

			for (int y = 0; y < 3; y++)
			{
				auto childY = Element::create("child");
				childY->setAttribute("x", to_string(x));
				childY->setAttribute("y", to_string(y));

				childX->addChild(childY);
			}

			auto text = Element::create("text");
			text->content = "Some content";

			childX->addChild(text);
		}

		// act
		stringstream ss;
		ss << root;
		Parser parser;
		
		auto result = parser.parse(ss);

		// assert
		CPPUNIT_ASSERT(result != nullptr);
	}
};