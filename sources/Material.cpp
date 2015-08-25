#include "TypeDefs.h"

#include "PropertiesHolder/PropertiesHolder.h"

#include "Material.h"

using namespace tfem;

Material::Material()
{
	m_properties.SetHolderName("Undefined material");
}

std::string Material::GetID()
{
	return m_properties.operator[]<std::string>("id");
};

float Material::GetPoissonsRatio()
{
	return m_properties.operator[]<float>("PoissonsRatio");
};

float Material::GetYoungsModulus()
{
	return m_properties.operator[]<float>("YoungsModulus");
};

inline Eigen::MatrixXf Material::GetElasticityMatrix(fem::ProblemType type)
{
	float nu = GetPoissonsRatio();
	float E = GetYoungsModulus();

	std::function<Eigen::MatrixXf()> flatStress = [nu, E]()
	{
		Eigen::Matrix<float, 3, 3> m;
		m.setIdentity();
		m(0, 1) = nu;
		m(1, 0) = nu;
		m(2, 2) = (1.0f - nu) / 2.0f;
		m *= E / (1.0f - nu * nu);
		return m;
	};
	std::function<Eigen::MatrixXf()> flatStrain = [nu, E]()
	{
		Eigen::Matrix<float, 3, 3> m;
		m.setIdentity();
		m(0, 1) = nu / (1.0f - nu);
		m(1, 0) = nu / (1.0f - nu);
		m(2, 2) = (1.0f - 2.0f * nu) / 2.0f / (1.0f - nu);
		m *= E * (1.0f - nu) / (1.0f + nu) / (1.0f - 2.0f * nu);
		return m;
	};
	std::function<Eigen::MatrixXf()> volumetricStressStrain = [nu, E]()
	{
		Eigen::Matrix<float, 6, 6> m;
		m.setIdentity();
		float tmp1 = nu / (1.0f - nu);
		float tmp2 = (1.0f - 2.0f * nu) / 2.0f / (1.0f - nu);
		m(0, 1) = m(1, 0) = m(2, 0) = m(0, 2) = m(2, 1) = m(1, 2) = tmp1;
		m(5, 5) = m(4, 4) = m(3, 3) = tmp2;
		m *= E * (1.0f - nu) / (1.0f + nu) / (1.0f - 2.0f * nu);
		return m;
	};

	switch (type)
	{
	case fem::PT_FlatStress:
		return flatStress();
	case fem::PT_FlatStraing:
		return flatStrain();
	case fem::PT_VolumetricStressStrain:
		return volumetricStressStrain();
	}
}
