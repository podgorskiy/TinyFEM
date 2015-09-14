#include "Common.h"
#include "Linear2DElement.h"

void Linear2DElement::SetIndices(const std::vector<int>& indices)
{
	assert(indices.size() == 3);
	m_nodes[0] = indices[0];
	m_nodes[1] = indices[1];
	m_nodes[2] = indices[2];
}

std::vector<int> Linear2DElement::GetIndices() const
{
	std::vector<int> indices(3);
	indices[0] = m_nodes[0];
	indices[1] = m_nodes[1];
	indices[2] = m_nodes[2];
	return indices;
}


IElement* Linear2DElement::Create()
{
	return new Linear2DElement;
}