#pragma once

#include "TypeDefs.h"

#include <Eigen/Dense>
#include <Eigen/StdVector>

namespace tfem
{
	class Material
	{
	public:
		Material();

		template <typename T>
		void PushProperty(const std::string& name, const T& value);

		template <typename T>
		bool DoesPropertyExists(const std::string& name) const;

		template <typename T>
		T operator[](const char* name) const;

		std::string GetID();

		float GetPoissonsRatio() const;

		float GetYoungsModulus() const;

		Eigen::MatrixXf GetElasticityMatrix(fem::ProblemType type) const;

	private:
		PropertiesHolder m_properties;
	};

	template <typename T>
	inline void Material::PushProperty(const std::string& name, const T& value)
	{
		return m_properties.PushProperty(name, value);
	};

	template <>
	inline void Material::PushProperty(const std::string& name, const std::string& value)
	{
		if (name == "id")
		{
			m_properties.SetHolderName("Material \""+value + "\"");
		}
		return m_properties.PushProperty(name, value);
	};

	template <typename T>
	inline bool Material::DoesPropertyExists(const std::string& name) const
	{
		return m_properties.DoesPropertyExists<T>(name);
	};

	template <typename T>
	inline T Material::operator[](const char* name) const
	{
		return m_properties.operator[]<T>(name);
	};

	typedef boost::shared_ptr<Material> MaterialPtr;
}
