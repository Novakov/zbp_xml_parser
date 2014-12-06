#include <string>
#include <map>
#include <vector>

class Element
{
private:
	std::string _name;
	std::vector<std::shared_ptr<Element>> children;
	std::map<std::string, std::string> attributes;
public:
	std::string content;

	Element(const std::string name) : _name(name)
	{}

	std::string name() { return this->_name; }

	void addChild(std::shared_ptr<Element> child);
	int childrenCount();
	std::shared_ptr<Element> child(int index);

	void setAttribute(std::string name, std::string value);
	int attributeCount();
	std::string attribute(std::string name);	
};