#include "Common.h"
#include "IElement.h"
#include "ElementFabric.h"
#include <cassert>
#include "CPS3.h"
#include "CPS4.h"


IElement* ElementFabric::CreateElement()
{
	if (m_activeGenerator != NULL)
	{
		return m_activeGenerator();
	}
	return NULL;
}

void ElementFabric::SetType(const std::string& type)
{
	m_activeGenerator = GetGenerator(type);
}

ElementFabric::ElementFabric() : m_activeGenerator(NULL)
{
	CPS4::Init();
	RegisterElementType<CPS3>();
	RegisterElementType<CPS4>();
}

ElementFabric::Generator ElementFabric::GetGenerator(const std::string& type)
{
	std::map<std::string, IElement*(*)()>::iterator it = m_generators.find(type);
	if (it != m_generators.end())
	{
		return it->second;
	}
	return NULL;
}

IElement* ElementFabric::CreateElement(const std::string& type)
{
	Generator g = GetGenerator(type);
	if (g == NULL)
	{
		LOGW("Unknown element type: %s \n", type.c_str());
		return NULL;
	}
	return g();
}

void ElementFabric::RegisterElementType(const std::string& typeName, IElement* (*function)())
{
	m_generators[typeName] = function;
}