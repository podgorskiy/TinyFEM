#include <map>

#include "Common.h"
#include "XMLDataUtils.h"
#include "Elements/IElement.h"
#include "Elements/ElementsContainer.h"
#include "Problem.h"

#include "MaterialReader.h"
#include "NodeReader.h"
#include "ElementReader.h"
#include "Load.h"
#include "LoadReader.h"
#include <Eigen/Dense>

#include "IProblemTypeSpecific.h"
#include "Problem2DTypeSpecific.h"

struct ProblemImpl
{
	void GenerateGlobalKMatrix();
	void ApplyConstrints();
	void CreateLoadVector();

	template<typename Solver>
	void SolveSLI(int maxIterationCount, float expsilon);
	
	StrideDataFixedArray			m_nodes;
	ElementsContainer				m_elements;
	MaterialsLibrary				m_matLib;
	IndexedStrideDataFixedArray		m_nodalForceList;
	NodalConstraintList				m_nodalConstraintList;
	ProblemDescription				m_desc;

	Eigen::SparseMatrix<float> m_globalK;
	Eigen::VectorXf m_input;
	Eigen::VectorXf m_output;

	void ReadNodes(TiXmlHandle hRoot);
	void ReadMaterials(TiXmlHandle hRoot);
	void ReadElements(TiXmlHandle hRoot);
	void ReadNodalForces(TiXmlHandle hRoot);
	void ReadNodalConstraint(TiXmlHandle hRoot);

	IProblemTypeSpecific* m_typeSpecific;
};

void ProblemImpl::GenerateGlobalKMatrix()
{
	int elementsCount = m_elements.GetCount();

	std::vector<Eigen::Triplet<float> > triplets;
	for (int i = 0; i < elementsCount; ++i)
	{
		m_elements(i)->CalcK(m_nodes, m_matLib.begin()->second, triplets);
	}

	int problemDimension = m_typeSpecific->GetProblemDimension(m_nodes.GetCount());
	m_globalK = Eigen::SparseMatrix<float>(problemDimension, problemDimension);
	m_globalK.setFromTriplets(triplets.begin(), triplets.end());
}

void SetConstraints(Eigen::SparseMatrix<float>::InnerIterator& it, int index)
{
	if (it.row() == index || it.col() == index)
	{
		it.valueRef() = it.row() == it.col() ? 1.0f : 0.0f;
	}
}

void ProblemImpl::ApplyConstrints()
{
	std::vector<int> indicesToConstraint;

	for (NodalConstraintList::iterator itc = m_nodalConstraintList.begin(); itc != m_nodalConstraintList.end(); ++itc)
	{
		m_typeSpecific->GrabConstraintIndices(itc->node, itc->type, indicesToConstraint);
	}

	for (int k = 0; k < m_globalK.outerSize(); ++k)
	{
		for (Eigen::SparseMatrix<float>::InnerIterator it(m_globalK, k); it; ++it)
		{
			for (std::vector<int>::iterator idit = indicesToConstraint.begin(); idit != indicesToConstraint.end(); ++idit)
			{
				SetConstraints(it, *idit);
			}
		}
	}
}

void ProblemImpl::CreateLoadVector()
{
	m_input = Eigen::VectorXf(m_typeSpecific->GetProblemDimension(m_nodes.GetCount()));
	m_input.setZero();

	int dof = m_typeSpecific->GetDOFPerNode();

	for (int i = 0, l = m_nodalForceList.GetCount(); i < l; ++i)
	{
		for (int j = 0; j < dof; ++j)
		{
			m_input[dof * m_nodalForceList[i] + j] = m_nodalForceList(i, j);
		}
	}
}

template<>
void ProblemImpl::SolveSLI<Eigen::ConjugateGradient<Eigen::SparseMatrix<float> > >(int maxIterationCount, float expsilon)
{
	Eigen::ConjugateGradient<Eigen::SparseMatrix<float> > solver(m_globalK);
	solver.setMaxIterations(maxIterationCount);
	solver.setTolerance(expsilon);
	m_output = solver.solve(m_input);
}

template<typename Solver>
void ProblemImpl::SolveSLI(int maxIterationCount, float expsilon)
{
	Solver solver(m_globalK);
	m_output = solver.solve(m_input);
}

void Problem::OpenFromMemory(const std::string& memoryString)
{
	xmldata::TiXmlDocumentPtr hDoc(new TiXmlDocument());
	hDoc->Parse(memoryString.c_str(), 0, TIXML_ENCODING_UTF8);
	OpenDoc(hDoc.get());
}

void Problem::OpenFromFile(const std::string& filename)
{
	xmldata::TiXmlDocumentPtr hDoc = xmldata::OpenXMLDoc(filename);
	OpenDoc(hDoc.get());
}

void Problem::SolveSLI()
{
	switch (m_type)
	{
	case ST_CholeskyLDLT:
		m_impl->SolveSLI<Eigen::SimplicialLDLT<Eigen::SparseMatrix<float> > >(m_maxIterationCount, m_expsilon);
		break;
	case ST_CholeskyLLT:
		m_impl->SolveSLI<Eigen::SimplicialLLT<Eigen::SparseMatrix<float> > >(m_maxIterationCount, m_expsilon);
		break;
	case ST_IterativeCG:
		m_impl->SolveSLI<Eigen::ConjugateGradient<Eigen::SparseMatrix<float> > >(m_maxIterationCount, m_expsilon);
		break;
	}
}

void Problem::SetSolverOptions(SolverType type, int maxIterationCount, float expsilon)
{
	m_type = type;
	m_maxIterationCount = maxIterationCount;
	m_expsilon = expsilon;
}

void Problem::SolveProblem()
{
	m_impl->m_typeSpecific->SolveProblem(*this);
}

void Problem::Clear()
{
	m_impl->m_elements.Init(0);
	m_impl->m_nodes.Init(0, 0);
	m_impl->m_matLib.clear();
	m_impl->m_nodalConstraintList.clear();
	m_impl->m_nodalForceList.Init(0, 0);
	m_impl->m_globalK = Eigen::SparseMatrix<float>();
	m_impl->m_input = Eigen::VectorXf();
	m_impl->m_output = Eigen::VectorXf();
}

void Problem::OpenDoc(TiXmlDocument* hDoc)
{
	Clear();
	TiXmlHandle hRoot(NULL);
	{
		TiXmlElement* pElem;
		pElem = hDoc->FirstChildElement();
		if (pElem == NULL)
		{
			LOGE("Document does not have root");
		}
		LOGI("Doc type: \t\t%s", pElem->Value());
		hRoot = TiXmlHandle(pElem);
	}

	LOGI(" ");

	m_impl->m_typeSpecific = new Problem2DTypeSpecific;

	/* Materials */
	m_impl->ReadMaterials(hRoot);

	/* Nodes */
	m_impl->ReadNodes(hRoot);

	/* Elements */
	m_impl->ReadElements(hRoot);

	/* NodalForces */
	m_impl->ReadNodalForces(hRoot);

	/* NodalConstraints */
	m_impl->ReadNodalConstraint(hRoot);
}

void Problem::SaveToFile(const std::string& filename) const
{

}

const ProblemDescription& Problem::GetProblemDescription() const
{
	return m_impl->m_desc;
}

const StrideDataFixedArray& Problem::GetNodes() const
{
	return m_impl->m_nodes;
}

const ElementsContainer& Problem::GetElements() const
{
	return m_impl->m_elements;
}

const Eigen::VectorXf& Problem::GetDeforms() const
{
	return m_impl->m_output;
}

const IndexedStrideDataFixedArray& Problem::GetNodalForceList() const
{
	return m_impl->m_nodalForceList;
}

const NodalConstraintList& Problem::GetNodalConstraintList() const
{
	return m_impl->m_nodalConstraintList;
}

void Problem::UpdateNodes(MeshRenderer* meshRenderer) const
{
	if (m_impl->m_typeSpecific != NULL)
		m_impl->m_typeSpecific->UpdateNodes(*this, meshRenderer);
}

void Problem::SubmitMeshesToRender(MeshRenderer* meshRenderer) const
{
	if (m_impl->m_typeSpecific != NULL)
		m_impl->m_typeSpecific->SubmitMeshesToRender(*this, meshRenderer);
}

bool Problem::UpdateViewOption(MeshRenderer* meshRenderer)
{
	if (m_impl->m_typeSpecific != NULL)
		return m_impl->m_typeSpecific->UpdateView(meshRenderer);
	return false;
}

Problem::Problem() :m_impl(new ProblemImpl)
{
	m_impl->m_typeSpecific = NULL;
}

Problem::~Problem()
{
	delete m_impl;
}

void Problem::GenerateGlobalKMatrix()
{
	m_impl->GenerateGlobalKMatrix();
}

void Problem::ApplyConstrints()
{
	m_impl->ApplyConstrints();
}

void Problem::CreateLoadVector()
{
	m_impl->CreateLoadVector();
}

void ProblemImpl::ReadMaterials(TiXmlHandle hRoot)
{
	TiXmlNode* materialLibrary = xmldata::GetNodeByPath(hRoot.Node(), "materials");
	xmldata::foreachChild(materialLibrary->ToElement(), "material", ReadMaterialFunctor(m_matLib));
}

void ProblemImpl::ReadNodes(TiXmlHandle hRoot)
{
	TiXmlNode* nodeElementList = xmldata::GetNodeByPath(hRoot.Node(), "NodeList");
	int nodesCount = 0;
	xmldata::ParseValue(nodeElementList->ToElement(), "count", nodesCount);
	m_nodes.Init(m_typeSpecific->GetDOFPerNode(), nodesCount);
	xmldata::foreachChild(nodeElementList->ToElement(), "node", ReadNodeFunctor(m_nodes));
}

void ProblemImpl::ReadElements(TiXmlHandle hRoot)
{
	ElementFabric fabric;
	TiXmlNode* elemetsElementList = xmldata::GetNodeByPath(hRoot.Node(), "ElementList");
	int elementsCount = 0;
	std::string type;
	xmldata::ParseValue(elemetsElementList->ToElement(), "count", elementsCount);
	m_elements.Init(elementsCount);
	xmldata::foreachChild(elemetsElementList->ToElement(), "element", ReadElementFunctor(m_elements, fabric));
}

void ProblemImpl::ReadNodalForces(TiXmlHandle hRoot)
{
	TiXmlNode* nodalForcesElementList = xmldata::GetNodeByPath(hRoot.Node(), "NodalForceList");
	int loadsCount = 0;
	xmldata::ParseValue(nodalForcesElementList->ToElement(), "count", loadsCount);
	m_nodalForceList.Init(m_typeSpecific->GetDOFPerNode(), loadsCount);
	xmldata::foreachChild(nodalForcesElementList->ToElement(), "force", ReadNodalForceFunctor(m_nodalForceList));
}

void ProblemImpl::ReadNodalConstraint(TiXmlHandle hRoot)
{
	TiXmlNode* nodalConstraintElementList = xmldata::GetNodeByPath(hRoot.Node(), "NodalConstraintList");
	xmldata::foreachChild(nodalConstraintElementList->ToElement(), "const", ReadNodalConstraintFunctor(m_nodalConstraintList));
}