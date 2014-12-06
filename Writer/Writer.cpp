// Writer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <dom.h>
#include <Writer.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
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

	cout << root;

	return 0;
}

