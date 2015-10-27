#pragma once

class StrideDataArray;
class TiXmlElement;

class ReadNodeFunctor
{
public:
	ReadNodeFunctor(StrideDataArray& nodes);
	void operator()(TiXmlElement* nodeElement);

private:
	StrideDataArray& nodes;
};