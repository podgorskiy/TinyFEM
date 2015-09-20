#include "Common.h"
#include "ElementsContainer.h"


ElementsContainer::ElementsContainer() :m_elementsCount(0), m_data(NULL)
{
}

void ElementsContainer::Init(int elementsCount)
{
	m_elementsCount = elementsCount;
	SafeDelete(m_data);
	if (elementsCount != 0)
	{
		m_data = new IElement*[m_elementsCount];
	}
}

ElementsContainer::~ElementsContainer()
{
	SafeDelete(m_data);
}

int ElementsContainer::GetCount() const
{
	return m_elementsCount;
}