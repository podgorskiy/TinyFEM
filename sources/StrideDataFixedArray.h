#pragma once
#include "Common.h"
#include <vector>

class StrideDataArray
{
private:
	StrideDataArray(const StrideDataArray& other){};
	void operator= (const StrideDataArray& other){};
public:
	StrideDataArray();
	~StrideDataArray();
	void Init(int nodeDOF, int nodeCount);
	float& operator ()(int i, int j);
	const float& operator ()(int i, int j) const;
	int GetCount() const;
	void MakeCopy(StrideDataArray& other) const;
	int GetDof() const;

protected:
	int m_dof;
	std::vector<float> m_data;
};

class IndexedStrideDataArray : public StrideDataArray
{
public:
	IndexedStrideDataArray();
	~IndexedStrideDataArray();
	void Init(int nodeDOF, int nodeCount);
	int& operator [](int i);
	int operator [](int i) const;
	void MakeCopy(IndexedStrideDataArray& other) const;

private:
	std::vector<int> m_idices;
};

inline StrideDataArray::StrideDataArray()
{
};

inline StrideDataArray::~StrideDataArray()
{
}

inline void StrideDataArray::Init(int nodeDOF, int nodeCount)
{
	m_dof = nodeDOF;
	m_data.clear();
	if (m_dof != 0 && nodeCount != 0)
	{
		m_data.resize(m_dof * nodeCount);
	}
}

inline float& StrideDataArray::operator()(int i, int j)
{
	return *(m_data.data() + i * m_dof + j);
}

inline const float& StrideDataArray::operator()(int i, int j) const
{
	return *(m_data.data() + i * m_dof + j);
}

inline int StrideDataArray::GetCount() const
{
	return m_data.size() / m_dof;
}

inline void StrideDataArray::MakeCopy(StrideDataArray& other) const
{
	other.m_data = m_data;
	other.m_dof = m_dof;
}

inline int StrideDataArray::GetDof() const
{
	return m_dof;
}

inline int& IndexedStrideDataArray::operator[](int i)
{
	return m_idices.data()[i];
}

inline int IndexedStrideDataArray::operator [](int i) const
{
	return m_idices.data()[i];
}

inline void IndexedStrideDataArray::Init(int nodeDOF, int nodeCount)
{
	StrideDataArray::Init(nodeDOF, nodeCount);
	m_idices.resize(nodeCount);
}

inline IndexedStrideDataArray::IndexedStrideDataArray() :m_idices(NULL)
{
};

inline IndexedStrideDataArray::~IndexedStrideDataArray()
{
}

inline void IndexedStrideDataArray::MakeCopy(IndexedStrideDataArray& other) const
{
	other.m_idices = m_idices;
	other.m_data = m_data;
	other.m_dof = m_dof;
}