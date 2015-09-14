#pragma once
#include "Elements/ElementFabric.h"

class ElementFabric;
class ElementsContainer;
class TiXmlElement;

class ReadElementFunctor
{
public:
	ReadElementFunctor(ElementsContainer& elements, ElementFabric& fabric);
	void operator()(TiXmlElement* nodeElement);

private:
	ElementFabric& fabric;
	ElementsContainer& elements;
};
