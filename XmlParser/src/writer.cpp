#include "stdafx.h"

#include "Writer.h"

using namespace std;

void XmlWriter::write(shared_ptr<Element> element, ostream& output)
{
	output << this->currentIndent << "<" << element->name();

	for (auto it = element->attributesBegin(); it != element->attributesEnd(); it++)
	{
		output << " " << it->first << "=\"" << it->second << "\"";
	}

	if (element->childrenCount() > 0 || !element->content.empty())
	{
		output << ">" << element->content << endl;

		this->currentIndent += "  ";

		for (auto it = element->childrenBegin(); it != element->childrenEnd(); it++)
		{
			this->write(*it, output);
		}

		this->currentIndent.erase(this->currentIndent.length() - 2);

		output << this->currentIndent << "</" << element->name() << ">" << endl;
	}
	else
	{
		output << " />" << endl;
	}
}