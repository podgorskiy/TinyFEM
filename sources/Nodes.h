#pragma once

class Nodes
{
public:
	void Init(int nodeDOF, int nodeCount);
	float& operator ()(int i, int j);

private:
	int m_dof;
	int m_nodeCount;
	float* m_data;
};

inline void Nodes::Init(int nodeDOF, int nodeCount)
{
    m_dof = nodeDOF;
    m_nodeCount = nodeCount;
    m_data = new float[m_dof * m_nodeCount];
}

inline float& Nodes::operator()(int i, int j)
{
    return * (m_data + i * m_dof + j);
}
