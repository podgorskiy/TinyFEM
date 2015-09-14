#include <vector>

#include "ElementReader.h"
#include "StrideDataFixedArray.h"
#include "XMLDataUtils.h"
#include "Elements/ElementsContainer.h"

ReadElementFunctor::ReadElementFunctor(ElementsContainer& elements, ElementFabric& fabric) :elements(elements), fabric(fabric){};

void ReadElementFunctor::operator()(TiXmlElement* nodeElement)
{
	int i = 0;
	bool hasI = xmldata::ParseValue(nodeElement, "i", i);
	std::string indicesString;
	bool hasIndices = xmldata::ParseValue(nodeElement, "indices", indicesString);

	if (hasI && hasIndices)
	{
		IElement* element = fabric.CreateElement();
		elements(i) = element;
		std::vector<int> indices;
		std::vector<std::string> indexVector = xmldata::split(indicesString, ' ');
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
