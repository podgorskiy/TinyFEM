#pragma once

class INodeList;
class IShapeList;

class IShape
{
public:
		
	class iterator
	{
	public:
		iterator(const std::vector<INodeID>& indices) :
			indices(indices),
			size(indices.size()),
			it(indices.begin())
		{
		};
		iterator(const std::vector<INodeID>& indices, std::vector<INodeID>::const_iterator it) :
			indices(indices),
			size(indices.size()),
			it(it)
		{
		};
		INodeID operator -> ()
		{
			return *it;
		};
		INodeID operator * ()
		{
			return *it;
		};
		iterator& operator++()
		{
			++it;
			return *this;
		};
		iterator operator++(int)
		{
			iterator tmp = *this;
			++it;
			return tmp;
		};
		iterator& operator--()
		{
			--it;
			return *this;
		};
		iterator operator--(int)
		{
			iterator tmp = *this;
			--it;
			return tmp;
		};
		bool operator ==(const iterator& other)
		{
			return it == other.it;
		};
		bool operator !=(const iterator& other)
		{
			return it != other.it;
		};
		iterator operator + (int i)
		{
			int offset = (it - indices.begin()) + i;
			offset = offset % size;
			return iterator(indices, indices.begin() + offset);
		};
		iterator operator - (int i)
		{
			int offset = (it - indices.begin()) - i;
			offset += ((-offset) / size + 1) * size;
			offset = offset % size;
			return iterator(indices, indices.begin() + offset);
		};
	private:
		std::vector<INodeID>::const_iterator it;
		const std::vector<INodeID>& indices;
		const int size;
	};

	IShape(){};
	IShape(const IShape& other){};
	virtual ~IShape(){};

	virtual const char* 					GetType()	{ return "None"; };

	virtual void 							AssignNodeList(const INodeList* nodeList) = 0;
	virtual void 							AssignShapeList(const IShapeList* nodeList) = 0;

	virtual void 							SetNodeIndices(const std::vector<INodeID>& indices) = 0;

	virtual const std::vector<IShapeID>& 	GetAdjacentShapes() const = 0;
	virtual INodeLID 						GetLocalIndexByNodeIndex(INodeID id) const = 0;
	virtual Eigen::VectorXd 				GetPositionByLocalIndex(INodeLID lid) const = 0;

	// container like api
	virtual INodeID							operator[](int i) const = 0;
	virtual iterator						begin() const = 0;
	virtual iterator						end() const = 0;
	virtual INodeID							front() const = 0;
	virtual INodeID							back() const = 0;
	virtual int								size() const = 0;
};