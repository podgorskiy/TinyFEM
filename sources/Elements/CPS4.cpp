#include "Common.h"
#include "CPS4.h"
#include "PropertiesHolder/PropertiesHolder.h"
#include "Material.h"
#include <Eigen/Dense>

#include "GaussQuadrature.h"


Eigen::Matrix<float, 4, 4> CPS4::m_C;
Eigen::Matrix<float, 4, 4> CPS4::m_IC;

float XI[4] = { -1.0, 1.0, 1.0, -1.0 };
float ETA[4] = { -1.0, -1.0, 1.0, 1.0 };

void CPS4::Init()
{
	//x0: -1; x1: 1; x2: 1; x3: -1
	//y0: -1; y1: -1; y2: 1; y3: 1
	m_C <<	Eigen::Vector4f(1.0,				1.0,				1.0,				1.0), 
			Eigen::Vector4f(XI[0],				XI[1],				XI[2],				XI[3]),			// x0, x1, x2, x3
			Eigen::Vector4f(ETA[0],				ETA[1],				ETA[2],				ETA[3]),		// y0, y1, y2, y3
			Eigen::Vector4f(XI[0] * ETA[0],		XI[1] * ETA[1],		XI[2] * ETA[2],		XI[3] * ETA[3]);		// x0y0, x1y1, x2y2, x3y3
	m_IC = m_C.inverse();
}

void CPS4::SetIndices(const std::vector<int>& indices)
{
	assert(indices.size() == 4);
	m_nodes[0] = indices[0];
	m_nodes[1] = indices[1];
	m_nodes[2] = indices[2];
	m_nodes[3] = indices[3];
}

std::vector<int> CPS4::GetIndices() const
{
	std::vector<int> indices(4);
	indices[0] = m_nodes[0];
	indices[1] = m_nodes[1];
	indices[2] = m_nodes[2];
	indices[3] = m_nodes[3];
	return indices;
}

std::vector<Eigen::Vector3f> CPS4::GetFunctionValuesAtNodes(const Eigen::VectorXf& deforms)const
{
	Eigen::Matrix<float, 8, 1> uv;
	std::vector<Eigen::Vector3f> output;
	for (int i = 0; i < 4; ++i)
	{
		uv[2 * i + 0] = deforms[2 * m_nodes[i] + 0];
		uv[2 * i + 1] = deforms[2 * m_nodes[i] + 1];
	}

	for (int i = 0; i < 4; i++)
	{
		Eigen::Matrix<float, 3, 8> B = GetB(XI[i], ETA[i] );
		Eigen::Vector3f strain = B * uv;	
		output.push_back(strain);
	}
	return output;
}

void CPS4::CalcK(const StrideDataArray& nodes, const tfem::MaterialPtr mat, std::vector<Eigen::Triplet<float> >& tripletVector)
{
	m_mat = mat;
	Eigen::Vector4f X;
	Eigen::Vector4f Y;
	for (int i = 0; i < 4; ++i)
	{
		X[i] = nodes(m_nodes[i], 0);
		Y[i] = nodes(m_nodes[i], 1);
	}

	m_KX = m_IC * X;
	m_KY = m_IC * Y;

	float area = 0;
	Eigen::Matrix<float, 8, 8> K;
	K.setZero();
	
	float xi, eta, w1, w2;
	for (int i = 0; GaussQuadrature::GetWeights<2>(i, xi, w1); i++)
	{
		for (int j = 0; GaussQuadrature::GetWeights<2>(j, eta, w2); j++)
		{			
			float w = w1 * w2;
			Eigen::Matrix<float, 2, 2> J = GetJ(xi, eta);

			Eigen::Matrix<float, 3, 8> B = GetB(xi, eta);

			K += B.transpose() * mat->GetElasticityMatrix(fem::PT_FlatStress) * B * J.determinant() * w;
			area += J.determinant() * w;
		}
	}

	GrabTriplets(K, tripletVector);
}

void CPS4::GrabTriplets(const Eigen::Matrix<float, 8, 8>& K, std::vector<Eigen::Triplet<float> >& tripletVector) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
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

tfem::Material* CPS4::GetMaterial()
{
	return m_mat.get();
}

IElement* CPS4::Create()
{
	return new CPS4;
}

CPS4::CPS4()
{

}

Eigen::Matrix<float, 1, 4> CPS4::GetP(float xi, float eta) const
{
	return Eigen::Matrix<float, 1, 4>(1, xi, eta, xi * eta);
}

Eigen::Matrix<float, 1, 4> CPS4::GetdPdxi(float xi, float eta) const
{
	return Eigen::Matrix<float, 1, 4>(0, 1, 0, eta);
}

Eigen::Matrix<float, 1, 4> CPS4::GetdPdeta(float xi, float eta) const
{
	return Eigen::Matrix<float, 1, 4>(0, 0, 1, xi);
}

Eigen::Matrix<float, 2, 2> CPS4::GetJ(float xi, float eta) const
{
	float dxdxi = GetdPdxi(xi, eta) * m_KX;
	float dydxi = GetdPdxi(xi, eta) * m_KY;
	float dxdeta = GetdPdeta(xi, eta) * m_KX;
	float dydeta = GetdPdeta(xi, eta) * m_KY;
	Eigen::Matrix<float, 2, 2> result;
	result << dxdxi, dydxi, dxdeta, dydeta;
	return result;
}

Eigen::Matrix<float, 3, 8> CPS4::GetB(float xi, float eta) const
{
	Eigen::Matrix<float, 3, 8> B;
	Eigen::Matrix<float, 2, 2> J = GetJ(xi, eta);
	Eigen::Matrix<float, 2, 2> IJ = J.inverse();

	Eigen::Matrix<float, 1, 4> dNdxi = GetdPdxi(xi, eta) * m_IC;
	Eigen::Matrix<float, 1, 4> dNdeta = GetdPdeta(xi, eta) * m_IC;

	for (int k = 0; k < 4; ++k)
	{
		Eigen::Matrix<float, 2, 1> dNkdxieta(dNdxi[k], dNdeta[k]); 
		Eigen::Matrix<float, 2, 1> dNkdxy = IJ * dNkdxieta;
		B(0, 2 * k + 0) = dNkdxy[0];
		B(0, 2 * k + 1) = 0;
		B(1, 2 * k + 0) = 0;
		B(1, 2 * k + 1) = dNkdxy[1];
		B(2, 2 * k + 0) = dNkdxy[1];
		B(2, 2 * k + 1) = dNkdxy[0];
	}
	return B;
}