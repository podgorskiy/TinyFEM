#pragma once

using namespace tfem;

template <typename T>
bool Material::DoesPropertyExists(const std::string& name)			
{ 
	return m_properties.DoesPropertyExists<T>(name); 
};

template <typename T>
T Material::operator[](const char* name)
{ 
	return m_properties.operator[]<T>(name);
};

template <typename T>
void Material::PushProperty(const std::string& name, const T& value)
{
	return m_properties.PushProperty(name, value);
};

template <>
void Material::PushProperty(const std::string& name, const std::string& value)
{
	return m_properties.PushProperty(name, value);
};
