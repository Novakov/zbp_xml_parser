#include "stdafx.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <vector>
#include <memory>

#include <dom.h>

using namespace std;

class DomTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(DomTest);
	CPPUNIT_TEST(ShouldIterateOverAllChildren);
	CPPUNIT_TEST_SUITE_END();

private:
	shared_ptr<Element> root;
	shared_ptr<Element> child_1;
	shared_ptr<Element> child_2;
	shared_ptr<Element> child_3;
public:
	void setUp()
	{
		root = Element::create("root");
		root->addChild(child_1 = Element::create("child_1"));
		root->addChild(child_2 = Element::create("child_2"));
		root->addChild(child_3 = Element::create("child_3"));
	}

	virtual void ShouldIterateOverAllChildren()
	{
		// arrange

		// act & assert
		auto it = root->childrenBegin();
		
		CPPUNIT_ASSERT_EQUAL(child_1.get(), it->get());
		it++;

		CPPUNIT_ASSERT_EQUAL(child_2.get(), it->get());
		it++;

		CPPUNIT_ASSERT_EQUAL(child_3.get(), it->get());
		it++;

		CPPUNIT_ASSERT(root->childrenEnd() == it);
	}
};