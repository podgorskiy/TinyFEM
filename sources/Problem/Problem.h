#pragma once
#include <string>
#include "StrideDataFixedArray.h"
#include "Load.h"
#include "Elements/ElementsContainer.h"

class MeshRenderer;

class ProblemDescription
{

};

class TiXmlDocument;

struct ProblemImpl;

class Problem
{
	Problem(const Problem& other){};
	void operator=(const Problem& other){};
public:
	enum SolverType
	{
		ST_CholeskyLDLT,
		ST_CholeskyLLT,
		ST_IterativeCG
	};

	Problem();
	~Problem();

	void OpenFromMemory(const std::string& memoryString);
	void OpenFromFile(const std::string& filename);

	void SolveProblem();

	void SolveSLI();
	void SetSolverOptions(SolverType type, int maxIterationCount, float expsilon);

	void Clear();

	void SaveToFile(const std::string& filename) const;
	const ProblemDescription& GetProblemDescription() const;
	const StrideDataArray& GetNodes() const;
	const ElementsContainer& GetElements() const;
	const Eigen::VectorXf& GetDeforms() const;
	const IndexedStrideDataArray& GetNodalForceList() const;
	const NodalConstraintList& GetNodalConstraintList() const;

	void UpdateNodes(MeshRenderer* meshRenderer) const;
	void SubmitMeshesToRender(MeshRenderer* m_meshRenderer) const;
	
	void GenerateGlobalKMatrix();
	void ApplyConstrints();
	void CreateLoadVector();

	bool UpdateViewOption(MeshRenderer* meshRenderer);
private:
	void OpenDoc(TiXmlDocument* hDoc);
	ProblemImpl* m_impl;

	SolverType m_type;
	int m_maxIterationCount;
	float m_expsilon;
};
