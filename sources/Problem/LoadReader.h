#pragma once
#include "Load.h"

class StrideDataFixedArray;
class TiXmlElement;

class ReadNodalForceFunctor
{
public:
	ReadNodalForceFunctor(NodalForceList& loads);
	void operator()(TiXmlElement* loadElement);

private:
	NodalForceList& loads;
};

class ReadNodalConstraintFunctor
{
public:
	ReadNodalConstraintFunctor(NodalConstraintList& nodes);
	void operator()(TiXmlElement* constElement);

private:
	NodalConstraintList& constraints;
};