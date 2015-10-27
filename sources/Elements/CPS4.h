#pragma once

#include "IElement.h"
#include "StrideDataFixedArray.h"
#include <vector>
#include <cassert>

class CPS4 : public IElement
{
public:
	CPS4();
	static void Init();
	virtual const char* GetType()	{ return "CPS4"; };
	virtual void SetIndices(const std::vector<int>& indices);
	virtual std::vector<int> GetIndices() const;
	virtual std::vector<Eigen::Vector3f> GetFunctionValuesAtNodes(const Eigen::VectorXf& deforms)const;

	static IElement* Create();

	virtual void CalcK(const StrideDataArray& nodes, const tfem::MaterialPtr mat, std::vector<Eigen::Triplet<float> >& tripletVector);

	virtual tfem::Material* GetMaterial();

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
	void GrabTriplets(const Eigen::Matrix<float, 8, 8>& K, std::vector<Eigen::Triplet<float> >& tripletVector) const;

	Eigen::Matrix<float, 1, 4> GetP(float xi, float eta) const;
	Eigen::Matrix<float, 1, 4> GetdPdxi(float xi, float eta) const;
	Eigen::Matrix<float, 1, 4> GetdPdeta(float xi, float eta) const;
	Eigen::Matrix<float, 2, 2> GetJ(float xi, float eta) const;
	Eigen::Matrix<float, 3, 8> GetB(float xi, float eta) const;

	static Eigen::Matrix<float, 4, 4> m_C;
	static Eigen::Matrix<float, 4, 4> m_IC;
	
	Eigen::Vector4f m_KX;
	Eigen::Vector4f m_KY;

	MaterialPtr m_mat;
	int	    m_nodes[4];
	float   m_area;
};
