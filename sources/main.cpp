#include "StdAfx.h"

#define SEMT_USE_STD_VECTOR 1
#define SEMT_DISABLE_PRINT 0

#include <semt/Semtfwd.h>

#include <semt/Semt.h>
#include <semt/Shortcuts.h>

#include "PropertiesHolder/PropertiesHolder.h"

#include "Material.h"

#include "IValueList.h"
#include "INodeLists.h"

#include "IShape.h"
#include "Shape2D.h"
#include "IElement.h"

#include "Node2DStructural.h"

#include <iostream>

using Eigen::MatrixXd;

int main()
{
	DVAR(Eta, 0);
	DVAR(Xi, 1);

#define  _1 SEMT::Expr<SEMT::Integer<1> >()
#define  _2 SEMT::Expr<SEMT::Integer<2> >()
#define  _3 SEMT::Expr<SEMT::Integer<3> >()
#define  _4 SEMT::Expr<SEMT::Integer<4> >()
#define  _9 SEMT::Expr<SEMT::Integer<9> >()
#define  _10 SEMT::Expr<SEMT::Integer<10> >()
#define  _32 SEMT::Expr<SEMT::Integer<12> >()

	auto Nc = _1 / _32 * (_1 + Xi) * (_1 + Eta) * (_9 *(Xi * Xi + Eta * Eta) - _10);
	auto Ns = _9 / _32 * (_1 + Xi) * (_1 - Eta * Eta) * (_1 + _9 * Eta);

	auto dNcdx = deriv_t(Nc, Xi);
	auto dNcdy = deriv_t(Nc, Eta);

	auto dNsdx = deriv_t(Ns, Xi);
	auto dNsdy = deriv_t(Ns, Eta);

	std::cout << dNcdx << std::endl << std::endl;
	std::cout << dNcdy << std::endl << std::endl;
	std::cout << dNsdx << std::endl << std::endl;
	std::cout << dNsdy << std::endl << std::endl;
	
	//SEMT::Array x_0(2);
	//x_0[0] = 0.1;
	//x_0[1] = 0.1;

	//std::cout << f << std::endl;
	//std::cout << SEMT::deriv_t(f, x) << std::endl;
	
	xmldata::TiXmlDocumentPtr hDoc = xmldata::OpenXMLDoc("1.xml");
	TiXmlHandle hRoot(NULL);

	{
		TiXmlElement* pElem;							
		pElem = hDoc->FirstChildElement();
		if(pElem == NULL)
		{
			LOGE("Document does not have root");
			return 0;
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

	LOGI("");

	TiXmlNode* materialLibrary = xmldata::GetNodeByPath(hRoot.Node(), "materials");
	
	typedef std::map<std::string, tfem::MaterialPtr> MaterialsLibrary;
	MaterialsLibrary materialsLibrary;

	xmldata::foreachChild(materialLibrary->ToElement(), "material", [&materialsLibrary](TiXmlElement* matElement)
	{
		std::string matId = matElement->Attribute("id");
		LOGI("Material: %s", matElement->Attribute("id"));

		tfem::MaterialPtr material(new tfem::Material);
		materialsLibrary[matId] = material;

		material->PushProperty("id", matId);
		xmldata::foreachChild(matElement, "field", [material](TiXmlElement* field) 
		{
			std::string name;
			xmldata::ParseValue(field, "name", name);
			double floatValue = 0.0f;
			if (xmldata::ParseValue(field, "value", floatValue))
			{
				material->PushProperty(name, floatValue);
				LOGI("\tproperty: \t%s: %f", name.c_str(), floatValue);
			}
			else
			{				
				LOGE("\tproperty: \t%s has erroneous property.", name.c_str());
				return;
			}
		});
		LOGI("");
	});
	
	NodeStructuralList nodesList;
	nodesList.Init(fem::PD_TwoDimensional);

	IVectorListPtr nodeCoordinatesList = nodesList.GetCoordinateList();

	TiXmlNode* nodeElementList = xmldata::GetNodeByPath(hRoot.Node(), "NodeList");
	xmldata::foreachChild(nodeElementList->ToElement(), "node", [&nodeCoordinatesList](TiXmlElement* nodeElement)
	{
		int i = 0;
		bool hasI = xmldata::ParseValue(nodeElement, "i", i);
		nodeCoordinatesList->PushNode(xmldata::ReadCoordinates<2>(nodeElement));
	});

	//typedef std::map<std::string, tfem::ElementPropertiesPtr> ElementLibrary;

	TiXmlNode* elementPropertiesElementList = xmldata::GetNodeByPath(hRoot.Node(), "ElementProperties");
	xmldata::foreachChild(elementPropertiesElementList->ToElement(), "elementType", [](TiXmlElement* nodeElement)
	{
		std::string id;
		std::string type = "Flat2DStructural";
		int nodesCount = 3;
		std::string gaussIntegration = "3x3";

		bool hasId = xmldata::ParseValue(nodeElement, "id", id);
		bool hasType = xmldata::ParseValue(nodeElement, "type", type);
		bool hasNodesCount = xmldata::ParseValue(nodeElement, "NodesCount", nodesCount);
		bool hasGaussIntegration = xmldata::ParseValue(nodeElement, "gaussIntegration", gaussIntegration);

		fem::GaussIntegration giType = fem::GI_3X3;
		if (gaussIntegration == "3x3")
		{
			giType = fem::GI_3X3;
		}
		else if (gaussIntegration == "4x4")
		{
			giType = fem::GI_4X4;
		}
		else if (gaussIntegration == "5x5")
		{
			giType = fem::GI_5X5;
		}
		else
		{
			LOGE("Wrong itegration type: %s", gaussIntegration.c_str());
		}
	});

	TiXmlNode* elemetsElementList = xmldata::GetNodeByPath(hRoot.Node(), "ElementList");
	xmldata::foreachChild(elemetsElementList->ToElement(), "element", [](TiXmlElement* nodeElement)
	{
		std::string typeId;
		std::string indices;
		float thickness;
		bool hasTypeId = xmldata::ParseValue(nodeElement, "typeId", typeId);
		bool hasIndices = xmldata::ParseValue(nodeElement, "indices", indices);
		bool hasThickness = xmldata::ParseValue(nodeElement, "thickness", thickness);
	});

	std::vector<INodeID> v;
	v.push_back(0);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(6);
	
	Shape2D triag(5);

	IShape* shape = &triag;

	shape->AssignNodeList(&nodesList);
	shape->SetNodeIndices(v);

	Shape2D::iterator it = shape->begin();
	Shape2D::iterator itEnd = shape->end();

	while (it != itEnd)
	{
		std::cout << *(it + 1) << std::endl;
		std::cout << *(it - 1) << std::endl;
		std::cout << *it++ << std::endl << std::endl;
	}

	std::cout << (*shape)[1] << std::endl;
	
	std::cout << shape->GetPositionByLocalIndex(3) << std::endl;

	return 0;
}