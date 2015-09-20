#pragma once
#include "Common.h"

class StrideDataFixedArray
{
private:
	StrideDataFixedArray(const StrideDataFixedArray& other){};
	void operator= (const StrideDataFixedArray& other){};
public:
	StrideDataFixedArray();
	~StrideDataFixedArray();
	void Init(int nodeDOF, int nodeCount);
	float& operator ()(int i, int j);
	const float& operator ()(int i, int j) const;
	int GetCount() const;
	void MakeCopy(StrideDataFixedArray& other) const;
	int GetDof() const;

protected:
	int m_dof;
	int m_nodeCount;
	float* m_data;
};

class IndexedStrideDataFixedArray : public StrideDataFixedArray
{
public:
	IndexedStrideDataFixedArray();
	~IndexedStrideDataFixedArray();
	void Init(int nodeDOF, int nodeCount);
	int& operator [](int i);
	int operator [](int i) const;
	void MakeCopy(IndexedStrideDataFixedArray& other) const;

private:
	int* m_idices;
};

inline StrideDataFixedArray::StrideDataFixedArray() :m_data(NULL)
{
};

inline StrideDataFixedArray::~StrideDataFixedArray()
{
	SafeDeleteArray(m_data);
}

inline void StrideDataFixedArray::Init(int nodeDOF, int nodeCount)
{
	SafeDeleteArray(m_data);
    m_dof = nodeDOF;
    m_nodeCount = nodeCount;
	if (m_dof != 0 && m_nodeCount != 0)
	{
		m_data = new float[m_dof * m_nodeCount];
	}
}

inline float& StrideDataFixedArray::operator()(int i, int j)
{
	return *(m_data + i * m_dof + j);
}

inline const float& StrideDataFixedArray::operator()(int i, int j) const
{
	return *(m_data + i * m_dof + j);
}

inline int StrideDataFixedArray::GetCount() const
{
	return m_nodeCount;
}

inline void StrideDataFixedArray::MakeCopy(StrideDataFixedArray& other) const
{
	other.Init(m_dof, m_nodeCount);
	for (int i = 0; i < m_nodeCount; ++i)
	{
		for (int j = 0; j < m_dof; ++j)
		{
			other(i, j) = operator()(i, j);
		}
	}
}

inline int StrideDataFixedArray::GetDof() const
{
	return m_dof;
}

inline int& IndexedStrideDataFixedArray::operator[](int i)
{
	return m_idices[i];
}

inline int IndexedStrideDataFixedArray::operator [](int i) const
{
	return m_idices[i];
}

inline void IndexedStrideDataFixedArray::Init(int nodeDOF, int nodeCount)
{
	StrideDataFixedArray::Init(nodeDOF, nodeCount);

	SafeDeleteArray(m_idices);
	if (m_dof != 0 && m_nodeCount != 0)
	{
		m_idices = new int[m_nodeCount];
	}
}

inline IndexedStrideDataFixedArray::IndexedStrideDataFixedArray() :m_idices(NULL)
{
};

inline IndexedStrideDataFixedArray::~IndexedStrideDataFixedArray()
{
	SafeDeleteArray(m_idices);
}

inline void IndexedStrideDataFixedArray::MakeCopy(IndexedStrideDataFixedArray& other) const
{
	other.Init(m_dof, m_nodeCount);
	for (int i = 0; i < m_nodeCount; ++i)
	{
		for (int j = 0; j < m_dof; ++j)
		{
			other(i, j) = operator()(i, j);
		}
		other[i] = operator[](i);
	}
}