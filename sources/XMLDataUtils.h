#pragma once
#include "Common.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <tinyxml.h>
#include <Eigen/Dense>
#include <Eigen/StdVector>

namespace xmldata
{
	typedef boost::shared_ptr<TiXmlDocument> TiXmlDocumentPtr;

	TiXmlDocumentPtr OpenXMLDoc(std::string filename);

	TiXmlNode* GetNodeByPath(TiXmlNode* node, const std::string& path);

	std::string GetNodeText(TiXmlNode* node, const std::string& path);

	inline void foreachChild(TiXmlElement* node, std::string field, std::function< void(TiXmlElement* pElem) > f)
	{
		for (TiXmlElement* pChild = node->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement())
		{
			if (pChild->Value() == field)
			{
				f(pChild);
			}
		}
	}

	template<typename T>
	inline bool ParseValue(const std::string& str, T& returnVal)
	{
        std::istringstream stream(str);
        return (stream >> returnVal && stream.eof());
	}

	template<typename T>
	inline bool ParseValue(TiXmlElement* nodeElement, const std::string& name, T& returnVal)
	{
		const char* str = nodeElement->Attribute(name.c_str());
		if(str == NULL)
		{
			return false;
		}
		else
		{
			return ParseValue(str, returnVal);
		}
	}

	template<>
	inline bool ParseValue<bool>(const std::string& str, bool& returnVal)
	{
        if(str == "true")
		{
			returnVal = true;
			return true;
		}
		else if(str == "false")
		{
			returnVal = false;
			return true;
		}
		return false;
	}

	template<int N>
	Eigen::VectorXd ReadCoordinates(TiXmlElement* nodeElement);
	template<>
	inline Eigen::VectorXd ReadCoordinates<2>(TiXmlElement* nodeElement)
	{
		Eigen::Vector2d position;
		bool hasX = xmldata::ParseValue(nodeElement, "x", position.x());
		bool hasY = xmldata::ParseValue(nodeElement, "y", position.y());
		if (!hasX || !hasY)
		{
			LOGE("Wrong coordinate set in node %s", nodeElement->Value());
		}
		return position;
	}

	template<>
	inline Eigen::VectorXd ReadCoordinates<3>(TiXmlElement* nodeElement)
	{
		Eigen::Vector3d position;
		bool hasX = xmldata::ParseValue(nodeElement, "x", position.x());
		bool hasY = xmldata::ParseValue(nodeElement, "y", position.y());
		bool hasZ = xmldata::ParseValue(nodeElement, "z", position.z());
		if (!hasX || !hasY || !hasZ)
		{
			LOGE("Wrong coordinate set in node %s", nodeElement->Value());
		}
		return position;
	}

}
