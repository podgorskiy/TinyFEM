#pragma once 
#include "IProblemTypeSpecific.h"
#include <Eigen/Dense>

class IElement;

class Problem2DTypeSpecific : public IProblemTypeSpecific
{
public:
	enum Display
	{
		D_U = 0,
		D_E,
		D_S,
	};
	enum Component
	{
		C_1 = 0,
		C_2,
		C_Magnitude,
		C_11 = 0,
		C_22,
		C_12,
		C_Max,
		C_Min,
		C_Mises,
	};

	virtual int GetProblemDimension(int nodesCount);
	virtual int GetDOFPerNode();
	virtual void GrabConstraintIndices(int node, NodalConstraint::ConstraintType type, std::vector<int> & indices);
	virtual void SolveProblem(Problem& problem);
	virtual void UpdateNodes(const Problem& problem, MeshRenderer* meshRenderer) const;
	virtual void SubmitMeshesToRender(const Problem& problem, MeshRenderer* meshRenderer) const;
	virtual bool UpdateView(MeshRenderer* meshRenderer);

private:
	float GetValue(const Eigen::Vector3f& input, IElement* element, const Eigen::Vector2f& uv) const;
	Display m_display;
	Component m_component;
};