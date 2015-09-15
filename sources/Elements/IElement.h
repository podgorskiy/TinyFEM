#pragma once
#include <vector>
#include "StrideDataFixedArray.h"
#include <Eigen/Sparse>

namespace tfem
{
	class Material;
};


class INodeList;

class IElement
{
	IElement(const IElement& other){};
public:
	IElement(){};
	virtual ~IElement(){};

	virtual const char* GetType()	{ return "None"; };
	virtual void SetIndices(const std::vector<int>& indices) = 0;
	virtual std::vector<int> GetIndices() const = 0;
	virtual void CalcK(const StrideDataFixedArray& nodes, const tfem::Material* mat) = 0;
	virtual void GrabTriplets(std::vector<Eigen::Triplet<float> >& tripletVector) const = 0;
};
