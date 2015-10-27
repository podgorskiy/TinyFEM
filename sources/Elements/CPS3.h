#pragma once

#include "IElement.h"
#include "StrideDataFixedArray.h"
#include <vector>
#include <cassert>

class CPS3 : public IElement
{
public:
	CPS3();
	virtual const char* GetType()	{ return "CPS3"; };
	virtual void SetIndices(const std::vector<int>& indices);
	virtual std::vector<int> GetIndices() const;
	virtual std::vector<Eigen::Vector3f> GetFunctionValuesAtNodes(const Eigen::VectorXf& deforms)const;

	static IElement* Create();

	virtual void CalcK(const StrideDataArray& nodes, const tfem::MaterialPtr mat, std::vector<Eigen::Triplet<float> >& tripletVector);

	virtual tfem::Material* GetMaterial();

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
	void GrabTriplets(const Eigen::Matrix<float, 6, 6>& K, std::vector<Eigen::Triplet<float> >& tripletVector) const;

	Eigen::Matrix<float, 3, 6> m_B;
	MaterialPtr m_mat;
	int	    m_nodes[3];
	float   m_area;
};
