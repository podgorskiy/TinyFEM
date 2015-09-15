#include "LoadReader.h"
#include "StrideDataFixedArray.h"
#include "XMLDataUtils.h"

ReadNodalForceFunctor::ReadNodalForceFunctor(NodalForceList& loads) :loads(loads){};

void ReadNodalForceFunctor::operator()(TiXmlElement* loadElement)
{
	int node = 0;
	if (xmldata::ParseValue(loadElement, "node", node))
	{
		Eigen::Vector2d r = xmldata::ReadCoordinates<2>(loadElement);
		NodalForce nf;
		nf.node = node;
		nf.x = r[0];
		nf.y = r[1];
		nf.z = 0;
		loads.push_back(nf);
	}
};

ReadNodalConstraintFunctor::ReadNodalConstraintFunctor(NodalConstraintList& constraints) :constraints(constraints){};

void ReadNodalConstraintFunctor::operator()(TiXmlElement* constElement)
{
	int node = 0;
	if (xmldata::ParseValue(constElement, "node", node))
	{
		std::string type;
		xmldata::ParseValue(constElement, "type", type);
		NodalConstraint nc;
		nc.node = node;
		if (type == "UX")
		{
			nc.type = NodalConstraint::UX;
		}
		else if (type == "UY")
		{
			nc.type = NodalConstraint::UY;
		}
		else if (type == "UXY")
		{
			nc.type = NodalConstraint::UXY;
		}
		constraints.push_back(nc);
	}
};
