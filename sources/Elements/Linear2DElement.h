#pragma once

#include "IElement.h"
#include <vector>
#include <cassert>

class Linear2DElement : public IElement
{
public:
	virtual const char* GetType()	{ return "Linear2D"; };
	virtual void SetIndices(const std::vector<int>& indices);
	virtual std::vector<int> GetIndices() const;

	static IElement* Create();

private:
	float   m_K[36];
	int	    m_nodes[3];
	float   m_area;
};
