#include "Common.h"
#include "CPS3.h"
#include "PropertiesHolder/PropertiesHolder.h"
#include "Material.h"
#include <Eigen/Dense>

void CPS3::SetIndices(const std::vector<int>& indices)
{
	assert(indices.size() == 3);
	m_nodes[0] = indices[0];
	m_nodes[1] = indices[1];
	m_nodes[2] = indices[2];
}

std::vector<int> CPS3::GetIndices() const
{
	std::vector<int> indices(3);
	indices[0] = m_nodes[0];
	indices[1] = m_nodes[1];
	indices[2] = m_nodes[2];
	return indices;
}

std::vector<Eigen::Vector3f> CPS3::GetFunctionValuesAtNodes(const Eigen::VectorXf& deforms)const
{
	Eigen::Matrix<float, 6, 1> uv;
	for (int i = 0; i < 3; ++i)
	{
		uv[2 * i + 0] = deforms[2 * m_nodes[i] + 0];
		uv[2 * i + 1] = deforms[2 * m_nodes[i] + 1];
	}

	Eigen::Vector3f strain = m_B * uv;
	
	std::vector<Eigen::Vector3f> output;
	for (int i = 0; i < 3; i++)
	{
		output.push_back(strain);
	}
	return output;
}

void CPS3::CalcK(const StrideDataFixedArray& nodes, const tfem::MaterialPtr mat, std::vector<Eigen::Triplet<float> >& tripletVector)
{
	m_mat = mat;
	Eigen::Vector3f x;
	Eigen::Vector3f y;
	for (int i = 0; i < 3; ++i)
	{
		x[i] = nodes(m_nodes[i], 0);
		y[i] = nodes(m_nodes[i], 1);
	}
	Eigen::Matrix3f C;
	C << Eigen::Vector3f(1.0f, 1.0f, 1.0f), x, y;

	float area = C.determinant() / 2.0f;

	Eigen::Matrix3f IC = C.inverse();
	
	for (int i = 0; i < 3; i++)
	{
		m_B(0, 2 * i + 0) = IC(1, i);
		m_B(0, 2 * i + 1) = 0.0f;
		m_B(1, 2 * i + 0) = 0.0f;
		m_B(1, 2 * i + 1) = IC(2, i);
		m_B(2, 2 * i + 0) = IC(2, i);
		m_B(2, 2 * i + 1) = IC(1, i);
	}
	Eigen::Matrix<float, 6, 6> K = m_B.transpose() * mat->GetElasticityMatrix(fem::PT_FlatStress) * m_B * area;
	GrabTriplets(K, tripletVector);
}

void CPS3::GrabTriplets(const Eigen::Matrix<float, 6, 6>& K, std::vector<Eigen::Triplet<float> >& tripletVector) const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Eigen::Triplet<float> trplt11(2 * m_nodes[i] + 0, 2 * m_nodes[j] + 0, K(2 * i + 0, 2 * j + 0));
			Eigen::Triplet<float> trplt12(2 * m_nodes[i] + 0, 2 * m_nodes[j] + 1, K(2 * i + 0, 2 * j + 1));
			Eigen::Triplet<float> trplt21(2 * m_nodes[i] + 1, 2 * m_nodes[j] + 0, K(2 * i + 1, 2 * j + 0));
			Eigen::Triplet<float> trplt22(2 * m_nodes[i] + 1, 2 * m_nodes[j] + 1, K(2 * i + 1, 2 * j + 1));

			tripletVector.push_back(trplt11);
			tripletVector.push_back(trplt12);
			tripletVector.push_back(trplt21);
			tripletVector.push_back(trplt22);
		}
	}
}

tfem::Material* CPS3::GetMaterial()
{
	return m_mat.get();
}

IElement* CPS3::Create()
{
	return new CPS3;
}

CPS3::CPS3()
{

}