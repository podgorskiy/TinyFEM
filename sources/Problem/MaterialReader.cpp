#include "MaterialReader.h"
#include "XMLDataUtils.h"

ReadFieldFunctor::ReadFieldFunctor(tfem::MaterialPtr material) :material(material){};

void ReadFieldFunctor::operator()(TiXmlElement* field)
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
}


ReadMaterialFunctor::ReadMaterialFunctor(MaterialsLibrary& materialsLibrary) :materialsLibrary(materialsLibrary){};

void ReadMaterialFunctor::operator()(TiXmlElement* matElement)
{
	std::string matId = matElement->Attribute("id");
	LOGI("Material: %s", matElement->Attribute("id"));

	tfem::MaterialPtr material(new tfem::Material);
	materialsLibrary[matId] = material;

	material->PushProperty("id", matId);
	xmldata::foreachChild(matElement, "field", ReadFieldFunctor(material));
	LOGI(" ");
}
