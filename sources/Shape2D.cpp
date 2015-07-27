#include "StdAfx.h"

#include "IShape.h"
#include "Shape2D.h"

#include "IValueList.h"
#include "INodeLists.h"

void Shape2D::AssignNodeList(const INodeList* nodeList)
{
	m_nodeList = nodeList;
}

void Shape2D::AssignShapeList(const IShapeList* shapeList)
{
	m_shapeList = shapeList;
}

void Shape2D::SetNodeIndices(const std::vector<INodeID>& indices)
{
	if (indices.size() != size())
	{
		LOGE("Wrong count of indices! Given %d indices, need %d", indices.size(), size());
	}
	m_indices = indices;
}

const std::vector<IShapeID>& Shape2D::GetAdjacentShapes() const
{
	return m_indices;
}

INodeLID Shape2D::GetLocalIndexByNodeIndex(INodeID id) const
{
	int i = 0;
	for (auto it = m_indices.begin(); it != m_indices.end(); ++it,++i)
	{
		if (*it == id)
		{
			return i;
		}
	}
	return -1;
}

Eigen::VectorXd Shape2D::GetPositionByLocalIndex(INodeLID lid) const
{
	IVectorListPtr list = m_nodeList->GetCoordinateList();
	Eigen::VectorXd v = list->operator[](this->operator[](lid));
	return v;
}

