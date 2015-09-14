#pragma once
#include "IElement.h"

typedef IElement* PIElement;

class ElementsContainer
{
	ElementsContainer(const ElementsContainer& other){};
	const ElementsContainer& operator()(const ElementsContainer& other){};
public:
	ElementsContainer();
	~ElementsContainer();

	void Init(int nodeCount);
	PIElement& operator ()(int i);
	const PIElement& operator ()(int i) const;

	int GetCount() const;

private:
	int m_elementsCount;
	PIElement* m_data;
};

inline PIElement& ElementsContainer::operator()(int i)
{
	return *(m_data + i);
}

inline const PIElement& ElementsContainer::operator()(int i) const
{
	return *(m_data + i);
}
