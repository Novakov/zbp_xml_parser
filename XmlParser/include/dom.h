#include <string>

class Element
{
private:
	std::string _name;
public:
	Element(const std::string name) : _name(name)
	{}

	std::string name() { return this->_name; }
};