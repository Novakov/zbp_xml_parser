#include <string>

#include <vector>

class Element
{
private:
	std::string _name;
	std::vector<std::shared_ptr<Element>> children;
public:
	Element(const std::string name) : _name(name)
	{}

	std::string name() { return this->_name; }

	void addChild(std::shared_ptr<Element> child);
	int childrenCount();
	std::shared_ptr<Element> child(int index);
};