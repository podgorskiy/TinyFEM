#pragma once
#include <string>
#include <map>

class IElement;

class ElementFabric
{
public:
	ElementFabric();

	IElement* CreateElement();
	void SetType(const std::string& type);

	template<typename T>
	inline void RegisterElementType()
	{
		T element;
		RegisterElementType(element.GetType(), T::Create);
	}

private:
	typedef IElement*(*Generator)();
	IElement* CreateElement(const std::string& type);
	Generator GetGenerator(const std::string& type);
	void RegisterElementType(const std::string& typeName, IElement* (*function)());
	std::map<std::string, Generator> m_generators;
	Generator m_activeGenerator;
};