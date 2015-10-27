#pragma once
#include "Load.h"

class StrideDataArray;
class IndexedStrideDataArray;
class TiXmlElement;

class ReadNodalForceFunctor
{
public:
	ReadNodalForceFunctor(IndexedStrideDataArray& loads);
	void operator()(TiXmlElement* loadElement);

private:
	int m_iterator;
	IndexedStrideDataArray& loads;
};

class ReadNodalConstraintFunctor
{
public:
	ReadNodalConstraintFunctor(NodalConstraintList& nodes);
	void operator()(TiXmlElement* constElement);

private:
	NodalConstraintList& constraints;
};