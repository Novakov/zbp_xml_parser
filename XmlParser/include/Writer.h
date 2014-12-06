#include<sstream>
#include<memory>
#include "dom.h"

class XmlWriter
{
private:
	std::string currentIndent;
public:
	XmlWriter() : currentIndent("")
	{}

	void write(std::shared_ptr<Element> element, std::ostream &output);
};