#pragma once

class IVectorList
{
public:
	IVectorList(){};
	virtual ~IVectorList(){};
	IVectorList(const IVectorList& other){};

	virtual void Clear() = 0;
	virtual void PushNode(const Eigen::VectorXd& position) = 0;
	virtual Eigen::VectorXd operator [](INodeID i) = 0;
};

typedef boost::shared_ptr<IVectorList> IVectorListPtr;

template<typename T>
class VectorXList : public IVectorList
{
public:
	virtual void Clear()
	{
		m_nodes.clear();
	};
	virtual void PushNode(const Eigen::VectorXd& position)
	{
		m_nodes.push_back(position);
	};
	virtual Eigen::VectorXd operator [](INodeID i)
	{
		return m_nodes[i];
	};

private:
	typedef std::vector<T, Eigen::aligned_allocator<T> > VectorXVector;

	VectorXVector m_nodes;
};

typedef VectorXList<Eigen::Matrix<double, 1, 1> >				ScalardList;
typedef VectorXList<Eigen::Matrix<double, 2, 1> >				Vector2dList;
typedef VectorXList<Eigen::Matrix<double, 3, 1> >				Vector3dList;
typedef VectorXList<Eigen::Matrix<double, 3, 1> >				Vector6dList;

typedef VectorXList<Eigen::Matrix<int, Eigen::Dynamic, 1> >		VectorXiList;