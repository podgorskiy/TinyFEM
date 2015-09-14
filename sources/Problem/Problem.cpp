#include <map>

#include "Common.h"
#include "XMLDataUtils.h"
#include "Elements/IElement.h"
#include "Elements/ElementsContainer.h"
#include "Problem.h"

#include "MaterialReader.h"
#include "NodeReader.h"
#include "ElementReader.h"

void Problem::OpenFromFile(const std::string& filename)
{
	xmldata::TiXmlDocumentPtr hDoc = xmldata::OpenXMLDoc(filename);
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

	MaterialsLibrary matLib;
	TiXmlNode* materialLibrary = xmldata::GetNodeByPath(hRoot.Node(), "materials");
	xmldata::foreachChild(materialLibrary->ToElement(), "material", ReadMaterialFunctor(matLib));
	
	TiXmlNode* nodeElementList = xmldata::GetNodeByPath(hRoot.Node(), "NodeList");
	int nodesCount = 0;
	xmldata::ParseValue(nodeElementList->ToElement(), "count", nodesCount);
	m_nodes.Init(2, nodesCount);
	xmldata::foreachChild(nodeElementList->ToElement(), "node", ReadNodeFunctor(m_nodes));

	ElementFabric fabric;

	TiXmlNode* elemetsElementList = xmldata::GetNodeByPath(hRoot.Node(), "ElementList");
	
	int elementsCount = 0;
	std::string type;
	xmldata::ParseValue(elemetsElementList->ToElement(), "count", elementsCount);
	xmldata::ParseValue(elemetsElementList->ToElement(), "type", type);
	
	fabric.SetType(type);
	m_elements.Init(elementsCount);
	xmldata::foreachChild(elemetsElementList->ToElement(), "element", ReadElementFunctor(m_elements, fabric));
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