#pragma once
#include <vector>
#include "StrideDataFixedArray.h"
#include "PropertiesHolder/PropertiesHolder.h"
#include "Material.h"
#include <Eigen/Sparse>
#include <Eigen/Dense>

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
	virtual std::vector<Eigen::Vector3f> GetFunctionValuesAtNodes(const Eigen::VectorXf& deforms)const = 0;
	virtual void CalcK(const StrideDataFixedArray& nodes, const tfem::MaterialPtr mat, std::vector<Eigen::Triplet<float> >& tripletVector) = 0;
	virtual tfem::Material* GetMaterial() = 0;
};
