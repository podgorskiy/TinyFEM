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

void Problem::OpenDoc(TiXmlDocument* hDoc)
{
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

	std::string author = xmldata::GetNodeText(hRoot.Node(), "asset/contributor/author");
	LOGI("Author:\t\t%s", author.c_str());
	std::string tool = xmldata::GetNodeText(hRoot.Node(), "asset/contributor/authoring_tool");
	LOGI("Authoring tool:\t%s", tool.c_str());
	std::string created = xmldata::GetNodeText(hRoot.Node(), "asset/created");
	LOGI("Document created:\t%s", created.c_str());
	std::string modified = xmldata::GetNodeText(hRoot.Node(), "asset/modified");
	LOGI("Document modified:\t%s", modified.c_str());

	LOGI(" ");

	/* Materials */
	MaterialsLibrary matLib;
	TiXmlNode* materialLibrary = xmldata::GetNodeByPath(hRoot.Node(), "materials");
	xmldata::foreachChild(materialLibrary->ToElement(), "material", ReadMaterialFunctor(matLib));

	/* Nodes */
	TiXmlNode* nodeElementList = xmldata::GetNodeByPath(hRoot.Node(), "NodeList");
	int nodesCount = 0;
	xmldata::ParseValue(nodeElementList->ToElement(), "count", nodesCount);
	m_nodes.Init(2, nodesCount);
	xmldata::foreachChild(nodeElementList->ToElement(), "node", ReadNodeFunctor(m_nodes));

	/* Elements */
	ElementFabric fabric;
	TiXmlNode* elemetsElementList = xmldata::GetNodeByPath(hRoot.Node(), "ElementList");
	int elementsCount = 0;
	std::string type;
	xmldata::ParseValue(elemetsElementList->ToElement(), "count", elementsCount);
	xmldata::ParseValue(elemetsElementList->ToElement(), "type", type);
	fabric.SetType(type);
	m_elements.Init(elementsCount);
	xmldata::foreachChild(elemetsElementList->ToElement(), "element", ReadElementFunctor(m_elements, fabric));

	/* NodalForces */
	TiXmlNode* nodalForcesElementList = xmldata::GetNodeByPath(hRoot.Node(), "NodalForceList");
	NodalForceList nodalForceList;
	xmldata::foreachChild(nodalForcesElementList->ToElement(), "force", ReadNodalForceFunctor(nodalForceList));

	/* NodalConstraints */
	TiXmlNode* nodalConstraintElementList = xmldata::GetNodeByPath(hRoot.Node(), "NodalConstraintList");
	NodalConstraintList nodalConstraintList;
	xmldata::foreachChild(nodalConstraintElementList->ToElement(), "const", ReadNodalConstraintFunctor(nodalConstraintList));
	
	std::vector<Eigen::Triplet<float> > triplets;
	for (int i = 0; i < elementsCount; ++i)
	{
		m_elements(i)->CalcK(m_nodes, matLib.begin()->second.get());
		m_elements(i)->GrabTriplets(triplets);
	}
	Eigen::SparseMatrix<float> globalK(2 * nodesCount, 2 * nodesCount);
	globalK.setFromTriplets(triplets.begin(), triplets.end());

	/* Apply constraints*/
	for (int k = 0; k < globalK.outerSize(); ++k)
	{
		for (Eigen::SparseMatrix<float>::InnerIterator it(globalK, k); it; ++it)
		{
			for (NodalConstraintList::iterator itc = nodalConstraintList.begin(); itc != nodalConstraintList.end(); ++itc)
			{
				int ix = 2 * itc->node;
				int iy = 2 * itc->node + 1;
				if (itc->type & NodalConstraint::UX)
				{
					if (it.row() == ix || it.col() == ix)
					{
						it.valueRef() = 0.0f;
					}
					if (it.row() == it.col() && it.col() == ix)
					{
						it.valueRef() = 1.0f;
					}
				}
				if (itc->type & NodalConstraint::UY)
				{
					if (it.row() == iy || it.col() == iy)
					{
						it.valueRef() = 0.0f;
					}
					if (it.row() == it.col() && it.col() == iy)
					{
						it.valueRef() = 1.0f;
					}
				}
			}
		}
	}

	std::cout << globalK << std::endl;

	Eigen::VectorXf load(2 * nodesCount);
	load.setZero();

	for (NodalForceList::iterator it = nodalForceList.begin(); it != nodalForceList.end(); ++it)
	{
		load[2 * it->node + 0] = it->x;
		load[2 * it->node + 1] = it->y;
	}

	std::cout << load;

	std::cout << std::endl;

	Eigen::SimplicialCholesky<Eigen::SparseMatrix<float> > chol(globalK);
	Eigen::VectorXf deforms = chol.solve(load);

	std::cout << deforms;

	/*HACK*/
	for (int i = 0; i < nodesCount; ++i)
	{
		m_nodes(i, 0) += deforms[2 * i + 0] * 10;
		m_nodes(i, 1) += deforms[2 * i + 1] * 10;
	}

}

void Problem::SaveToFile(const std::string& filename) const
{

}

const ProblemDescription& Problem::GetProblemDescription() const
{
	return m_desc;
}

const StrideDataFixedArray& Problem::GetNodes() const
{
	return m_nodes;
}

const ElementsContainer& Problem::GetElements() const
{
	return m_elements;
}

Problem::Problem()
{
}

Problem::~Problem()
{
}