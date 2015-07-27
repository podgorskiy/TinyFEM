#pragma once
using namespace tfem;

class PropertiesHolder::PropertiesHolderImpl
{
	class Property
	{
	public:
		Property(){};
		template<typename T>
		Property(const T& v)
		{
			m_property = v;
		};
		template<typename T>
		bool CheckType() const
		{
			return boost::any_cast<T>(&m_property) != NULL;
		};
		template<typename T>
		explicit operator T() const
		{
			return boost::any_cast<T>(m_property);
		};
	private:
		boost::any m_property;
	};
		
public:
	template <typename T>
	void PushProperty(const std::string& name, const T& value);
	
	template <typename T>
	bool DoesPropertyExists(const std::string& name) const;
	
	template <typename T>
	T operator[](const char* name) const;
	void SetHolderName(const std::string& name);
private:
	std::map<std::string, Property> m_properties;
	std::string m_name;
};

inline PropertiesHolder::PropertiesHolder() :m_impl(new PropertiesHolder::PropertiesHolderImpl())
{
}

template <typename T>
inline void PropertiesHolder::PropertiesHolderImpl::PushProperty(const std::string& name, const T& value)
{
	m_properties[name] = value;
}

template <typename T>
inline bool PropertiesHolder::PropertiesHolderImpl::DoesPropertyExists(const std::string& name) const
{
	auto it = m_properties.find(name);
	if (it != m_properties.end())
	{
		return it->second.CheckType<T>();
	}
	return false;
};

template <typename T>
inline T PropertiesHolder::PropertiesHolderImpl::operator[](const char* name) const
{
	std::map<std::string, Property>::const_iterator it = m_properties.find(name);
	if (it != m_properties.end())
	{
		if (it->second.CheckType<T>())
		{
			return T(it->second);
		}
		LOGE("Properties holder %s: property %s have wrong type.", m_name.c_str(), name);
	}
	LOGE("Properties holder %s: property %s not found", m_name.c_str(), name);
	return T(NULL);
}

inline void PropertiesHolder::PropertiesHolderImpl::SetHolderName(const std::string& name)
{
	m_name = name;
}

template <typename T>
inline void PropertiesHolder::PushProperty(const std::string& name, const T& value)
{
	m_impl->PushProperty(name, value);
}
	
template <typename T>
inline bool PropertiesHolder::DoesPropertyExists(const std::string& name) const
{
	return m_impl->DoesPropertyExists<T>(name);
}
	
template <typename T>
inline T PropertiesHolder::operator[](const char* name) const
{
	return m_impl->operator[]<T>(name);
}
	
inline void PropertiesHolder::SetHolderName(const std::string& name)
{
	m_impl->SetHolderName(name);
}
