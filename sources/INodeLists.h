#pragma once

class INodeList
{
public:
	INodeList(){};
	virtual ~INodeList(){};
	INodeList(const INodeList& other){};

	IVectorListPtr GetCoordinateList() const		{ return m_coordinateList; };

	virtual void Init(fem::ProblemDimension d) = 0;

protected:
	IVectorListPtr m_coordinateList;
};

class NodeStructuralList: public INodeList
{
public:
	NodeStructuralList(){};
	virtual ~NodeStructuralList(){};
	NodeStructuralList(const NodeStructuralList& other){};
	
	virtual void Init(fem::ProblemDimension d)
	{
		std::function<IVectorList*()> firstOrderF;
		std::function<IVectorList*()> secondOrderF;
		if (d == fem::PD_TwoDimensional)
		{
			firstOrderF = [](){return new Vector2dList(); };
			secondOrderF = [](){return new Vector3dList(); };
		}
		if (d == fem::PD_ThreeDimensional)
		{
			firstOrderF = [](){return new Vector3dList(); };
			secondOrderF = [](){return new Vector6dList(); };
		}
		m_coordinateList = IVectorListPtr(firstOrderF());
		m_displacementList = IVectorListPtr(firstOrderF());
		
		m_strainList = IVectorListPtr(secondOrderF());
		m_stressList = IVectorListPtr(secondOrderF());
		
		m_massList			= IVectorListPtr(new ScalardList());
	};

	IVectorListPtr GetDisplacementList()	{ return m_displacementList; };
	IVectorListPtr GetStrainList()			{ return m_strainList; };
	IVectorListPtr GetStressList()			{ return m_stressList; };
	IVectorListPtr GetMassList()			{ return m_massList; }

private:
	IVectorListPtr m_displacementList;
	IVectorListPtr m_strainList;
	IVectorListPtr m_stressList;
	IVectorListPtr m_massList;
};

