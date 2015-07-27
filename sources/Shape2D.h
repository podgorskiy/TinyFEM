#pragma once

class INodeList;
class IShapeList;

class Shape2D: public IShape
{
public:
	Shape2D(int n) :m_nodesCount(n){};

	const char* 					GetType()	{ return "Shape2D"; };

	void 							AssignNodeList(const INodeList* nodeList);
	void 							AssignShapeList(const IShapeList* shapeList);
	
	void 							SetNodeIndices(const std::vector<INodeID>& indices);

	const std::vector<IShapeID>& 	GetAdjacentShapes() const;
	INodeLID 						GetLocalIndexByNodeIndex(INodeID id) const;
	Eigen::VectorXd 				GetPositionByLocalIndex(INodeLID lid) const; 

	// conteiner like api
	INodeID							operator[](int i) const
	{
		int p = (i + m_nodesCount) % m_nodesCount;
		return m_indices[p];
	};
	iterator						begin() const
	{
		return iterator(m_indices);
	};
	iterator						end() const
	{
		return iterator(m_indices, m_indices.end());
	};
	INodeID							front() const
	{
		return m_indices.front();
	};
	INodeID							back() const
	{
		return m_indices.back();
	};
	int								size() const
	{
		return m_nodesCount;
	};

private:
	const int m_nodesCount;
	const INodeList* m_nodeList;
	const IShapeList* m_shapeList;
	std::vector<INodeID> m_indices;
};
