#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

class Element
{
private:
	std::string _name;
	std::vector<std::shared_ptr<Element>> children;
	std::map<std::string, std::string> attributes;
public:
	typedef std::vector<std::shared_ptr<Element>>::iterator ChildrenIterator;
	typedef std::map<std::string, std::string>::iterator AttributesIterator;

	std::string content;

	Element(const std::string name) : _name(name)
	{}

	static std::shared_ptr<Element> create(std::string name);

	std::string name() { return this->_name; }

	void addChild(std::shared_ptr<Element> child);
	size_t childrenCount();
	std::shared_ptr<Element> child(size_t index);
	void removeChild(size_t index);

	void setAttribute(std::string name, std::string value);
	size_t attributeCount();
	std::string attribute(std::string name);	
	
	Element::ChildrenIterator childrenBegin();
	Element::ChildrenIterator childrenEnd();

	Element::AttributesIterator attributesBegin();
	Element::AttributesIterator attributesEnd();

	friend std::ostream& operator<<(std::ostream& output, const std::shared_ptr<Element> element);
};

