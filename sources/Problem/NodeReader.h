#pragma once

class StrideDataFixedArray;
class TiXmlElement;

class ReadNodeFunctor
{
public:
	ReadNodeFunctor(StrideDataFixedArray& nodes);
	void operator()(TiXmlElement* nodeElement);

private:
	StrideDataFixedArray& nodes;
};