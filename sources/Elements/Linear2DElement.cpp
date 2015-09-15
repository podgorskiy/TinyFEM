#include "Common.h"
#include "Linear2DElement.h"
#include "PropertiesHolder/PropertiesHolder.h"
#include "Material.h"
#include <Eigen/Dense>

void Linear2DElement::SetIndices(const std::vector<int>& indices)
{
	assert(indices.size() == 3);
	m_nodes[0] = indices[0];
	m_nodes[1] = indices[1];
	m_nodes[2] = indices[2];
}

std::vector<int> Linear2DElement::GetIndices() const
{
	std::vector<int> indices(3);
	indices[0] = m_nodes[0];
	indices[1] = m_nodes[1];
	indices[2] = m_nodes[2];
	return indices;
}

void Linear2DElement::CalcK(const StrideDataFixedArray& nodes, const tfem::Material* mat)
{
	Eigen::Vector3f x;
	Eigen::Vector3f y;
	for (int i = 0; i < 3; ++i)
	{
		x[i] = nodes(m_nodes[i], 0);
		y[i] = nodes(m_nodes[i], 1);
	}
	Eigen::Matrix3f C;
	C << Eigen::Vector3f(1.0f, 1.0f, 1.0f), x, y;
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

	m_K = m_B.transpose() * mat->GetElasticityMatrix(fem::PT_FlatStress) * m_B;
}

void Linear2DElement::GrabTriplets(std::vector<Eigen::Triplet<float> >& tripletVector) const
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Eigen::Triplet<float> trplt11(2 * m_nodes[i] + 0, 2 * m_nodes[j] + 0, m_K(2 * i + 0, 2 * j + 0));
			Eigen::Triplet<float> trplt12(2 * m_nodes[i] + 0, 2 * m_nodes[j] + 1, m_K(2 * i + 0, 2 * j + 1));
			Eigen::Triplet<float> trplt21(2 * m_nodes[i] + 1, 2 * m_nodes[j] + 0, m_K(2 * i + 1, 2 * j + 0));
			Eigen::Triplet<float> trplt22(2 * m_nodes[i] + 1, 2 * m_nodes[j] + 1, m_K(2 * i + 1, 2 * j + 1));

			tripletVector.push_back(trplt11);
			tripletVector.push_back(trplt12);
			tripletVector.push_back(trplt21);
			tripletVector.push_back(trplt22);
		}
	}
}

IElement* Linear2DElement::Create()
{
	return new Linear2DElement;
}