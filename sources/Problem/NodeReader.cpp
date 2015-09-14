#include "NodeReader.h"
#include "StrideDataFixedArray.h"
#include "XMLDataUtils.h"

ReadNodeFunctor::ReadNodeFunctor(StrideDataFixedArray& nodes) :nodes(nodes){};

void ReadNodeFunctor::operator()(TiXmlElement* nodeElement)
{
	int i = 0;
	bool hasI = xmldata::ParseValue(nodeElement, "i", i);
	Eigen::Vector2d r = xmldata::ReadCoordinates<2>(nodeElement);
	nodes(i, 0) = r(0);
	nodes(i, 1) = r(1);
};
