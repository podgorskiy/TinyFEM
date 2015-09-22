#define _SCL_SECURE_NO_WARNINGS

#include <vector>
#include "ElementReader.h"
#include "StrideDataFixedArray.h"
#include "XMLDataUtils.h"
#include "Elements/ElementsContainer.h"
#include <boost/algorithm/string/split.hpp>

ReadElementFunctor::ReadElementFunctor(ElementsContainer& elements, ElementFabric& fabric) :elements(elements), fabric(fabric){};

void ReadElementFunctor::operator()(TiXmlElement* nodeElement)
{
	int i = 0;
	bool hasI = xmldata::ParseValue(nodeElement, "i", i);
	std::string indicesString;
	bool hasIndices = xmldata::ParseValue(nodeElement, "indices", indicesString);

	std::string type;
	xmldata::ParseValue(nodeElement, "type", type);
	fabric.SetType(type);

	if (hasI && hasIndices)
	{
		boost::trim(indicesString);
		IElement* element = fabric.CreateElement();
		elements(i) = element;
		std::vector<int> indices;
		std::vector<std::string> indexVector;
		boost::split(indexVector, indicesString, boost::is_any_of(" \t"));
		for (std::vector<std::string>::const_iterator it = indexVector.begin(); it != indexVector.end(); ++it)
		{
			int value = -1;
			bool parsed = xmldata::ParseValue(*it, value);
			if (parsed)
			{
				indices.push_back(value);
			}
		}
		elements(i)->SetIndices(indices);
	}
}
