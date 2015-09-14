#pragma once
#include <map>

#include "PropertiesHolder/PropertiesHolder.h"
#include "Material.h"

class TiXmlElement;

typedef std::map<std::string, tfem::MaterialPtr> MaterialsLibrary;

class ReadFieldFunctor
{
public:
	ReadFieldFunctor(tfem::MaterialPtr material);
	void operator()(TiXmlElement* field);
private:
	tfem::MaterialPtr material;
};


class ReadMaterialFunctor
{
public:
	ReadMaterialFunctor(MaterialsLibrary& materialsLibrary);
	void operator()(TiXmlElement* matElement);
private:
	MaterialsLibrary& materialsLibrary;
};