#pragma once

namespace tfem
{
	/*!
	* Class that holds generic type properties, that can be accessed by std::string key.
	* Does not throw exceptions. Use "DoesPropertyExists" to check if the property exists
	*/
	class PropertiesHolder
	{
	public:
		PropertiesHolder();

		template <typename T>
		void PushProperty(const std::string& name, const T& value);

		template <typename T>
		bool DoesPropertyExists(const std::string& name) const;

		template <typename T>
		T operator[](const char* name) const;

		void SetHolderName(const std::string& name);

	private:
		class PropertiesHolderImpl;
		boost::shared_ptr<PropertiesHolderImpl> m_impl;
	};
}

#include "PropertiesHolderImpl.h"