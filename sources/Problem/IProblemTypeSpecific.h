#pragma once

#include "Load.h"

class Problem;
class MeshRenderer;

class IProblemTypeSpecific
{
public:
	virtual int GetProblemDimension(int nodesCount) = 0;
	virtual int GetDOFPerNode() = 0;
	virtual void GrabConstraintIndices(int node, NodalConstraint::ConstraintType type, std::vector<int> & indices) = 0;

	virtual void SolveProblem(Problem& problem) = 0;

	virtual bool UpdateView(MeshRenderer* meshRenderer) = 0;

	virtual void UpdateNodes(const Problem& problem, MeshRenderer* meshRenderer) const = 0;
	virtual void SubmitMeshesToRender(const Problem& problem, MeshRenderer* meshRenderer) const = 0;
};
