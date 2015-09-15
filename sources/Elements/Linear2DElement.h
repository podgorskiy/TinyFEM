#pragma once

#include "IElement.h"
#include "StrideDataFixedArray.h"
#include <vector>
#include <cassert>

class Linear2DElement : public IElement
{
public:
	virtual const char* GetType()	{ return "Linear2D"; };
	virtual void SetIndices(const std::vector<int>& indices);
	virtual std::vector<int> GetIndices() const;

	static IElement* Create();

	virtual void CalcK(const StrideDataFixedArray& nodes, const tfem::Material* mat);
	virtual void GrabTriplets(std::vector<Eigen::Triplet<float> >& tripletVector) const;

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
	Eigen::Matrix<float, 3, 6> m_B;
	Eigen::Matrix<float, 6, 6> m_K;
	int	    m_nodes[3];
	float   m_area;
};
