#pragma once
#include "Common.h"
#include <string>
#include <sstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <tinyxml.h>
#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <boost/algorithm/string/trim.hpp>

namespace xmldata
{
	typedef boost::shared_ptr<TiXmlDocument> TiXmlDocumentPtr;

	TiXmlDocumentPtr OpenXMLDoc(std::string filename);

	TiXmlNode* GetNodeByPath(TiXmlNode* node, const std::string& path);

	std::string GetNodeText(TiXmlNode* node, const std::string& path);

	template <typename T>
	inline void foreachChild(TiXmlElement* node, std::string field, T functor)
	{
		for (TiXmlElement* pChild = node->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement())
		{
			if (pChild->Value() == field)
			{
				functor(pChild);
			}
		}
	}

	template<typename T>
	inline bool ParseValue(const std::string& str, T& returnVal)
	{
		std::istringstream stream(str);
		return (stream >> returnVal && stream.eof());
	}

	template<>
	inline bool ParseValue<std::string>(const std::string& str, std::string& returnVal)
	{
		returnVal = str;
		return true;
	}

	template<typename T>
	inline bool ParseValue(TiXmlElement* nodeElement, const std::string& name, T& returnVal)
	{
		const char* str_c = nodeElement->Attribute(name.c_str());
		if (str_c == NULL)
		{
			return false;
		}
		else
		{
			std::string str = str_c;
			boost::trim(str);
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