#include "stdafx.h"
#include<memory>

#include "dom.h"

using namespace std;

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