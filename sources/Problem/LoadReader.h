#pragma once
#include "Load.h"

class StrideDataFixedArray;
class IndexedStrideDataFixedArray;
class TiXmlElement;

class ReadNodalForceFunctor
{
public:
	ReadNodalForceFunctor(IndexedStrideDataFixedArray& loads);
	void operator()(TiXmlElement* loadElement);

private:
	int m_iterator;
	IndexedStrideDataFixedArray& loads;
};

class ReadNodalConstraintFunctor
{
public:
	ReadNodalConstraintFunctor(NodalConstraintList& nodes);
	void operator()(TiXmlElement* constElement);

private:
	NodalConstraintList& constraints;
};