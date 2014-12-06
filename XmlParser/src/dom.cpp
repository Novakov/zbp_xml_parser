#include "stdafx.h"
#include<memory>

#include "dom.h"

using namespace std;

shared_ptr<Element> Element::create(string name)
{
	return make_shared<Element>(name);
}

void Element::addChild(shared_ptr<Element> child)
{
	this->children.push_back(child);
}

int Element::childrenCount()
{
	return this->children.size();
}

shared_ptr<Element> Element::child(int index)
{
	return this->children.at(index);
}

int Element::attributeCount()
{
	return this->attributes.size();
}

string Element::attribute(string name)
{
	return this->attributes.at(name);
}


void Element::setAttribute(std::string name, std::string value)
{
	this->attributes[name] = value;
}

Element::ChildrenIterator Element::childrenBegin()
{
	return this->children.begin();
}

Element::ChildrenIterator Element::childrenEnd()
{
	return this->children.end();
}