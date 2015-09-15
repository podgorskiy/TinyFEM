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

	void MakeCopy(StrideDataFixedArray& other) const
	{
		other.Init(m_dof, m_nodeCount);
		for (int i = 0; i < m_nodeCount; ++i)
		{
			for (int j = 0; j < m_dof; ++j)
			{
				other(i, j) = operator()(i, j);
			}
		}
	};

private:
	int m_dof;
	int m_nodeCount;
	float* m_data;
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
    m_data = new float[m_dof * m_nodeCount];
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