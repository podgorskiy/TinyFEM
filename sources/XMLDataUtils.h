#pragma once

namespace xmldata
{
	typedef boost::shared_ptr<TiXmlDocument> TiXmlDocumentPtr;

	inline TiXmlDocumentPtr OpenXMLDoc(std::string filename)
	{
		typedef boost::shared_ptr<std::ifstream> IFStreamPtr; 
		IFStreamPtr input(new std::ifstream, [](std::ifstream* stream)
		{ 
			stream->close();
			delete stream;	
		});

		input->open(filename);
	
		std::streampos begin,end;
		begin = input->tellg();
		input->seekg(0, std::ios::end);
		end = input->tellg();
		input->seekg(0, std::ios::beg);
		std::streamoff s = end - begin;

		boost::scoped_ptr<char> buffer(new char[s]);
		input->read(buffer.get(), s);

		TiXmlDocument * xml = new TiXmlDocument;

		xml->Parse(buffer.get(), 0, TIXML_ENCODING_UTF8);

		return TiXmlDocumentPtr(xml);
	}

	inline TiXmlNode* GetNodeByPath(TiXmlNode* node, const std::string& path)
	{
		std::vector<std::string> pathSplitted;
		typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep("./");
		tokenizer tokens(path, sep);
		TiXmlNode* nodeCurrent = node;
		for(auto tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
		{
			nodeCurrent = nodeCurrent->FirstChild(*tok_iter);
			if(nodeCurrent == NULL)
			{
				return NULL;
			}
		}
		return nodeCurrent;
	}

	inline std::string GetNodeText(TiXmlNode* node, const std::string& path)
	{
		TiXmlNode* nodeOfInterest = GetNodeByPath(node, path);
		if (nodeOfInterest == NULL)
		{
			return "";
		}
		TiXmlNode* textNode = nodeOfInterest->FirstChild();
		if (textNode == NULL)
		{
			return "";
		}
		if (textNode->Type() != TiXmlNode::TINYXML_TEXT)
		{
			return "";
		}
		return textNode->Value();
	}
	
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
	inline bool ParseValue(const std::string& str, T& returnVal);

	template<typename T>
	inline bool ParseValue(const std::string& str, T& returnVal, std::function<T(const char*, char**)> f)
	{		
		char* pEnd = NULL;
		T value = f(str.c_str(), &pEnd);
		if(*pEnd == NULL)
		{
			returnVal = value;
			return true;
		}
		return false;
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
	inline bool ParseValue<int>(const std::string& str, int& returnVal)
	{
		return ParseValue<int>(str, returnVal, [](const char* strValue, char** returnValue){
			return strtol(strValue, returnValue, 10);
		});
	}

	template<>
	inline bool ParseValue<float>(const std::string& str, float& returnVal)
	{
		return ParseValue<float>(str, returnVal, [](const char* strValue, char** ret){
			return static_cast<float>(strtod(strValue, ret));
		});
	}

	template<>
	inline bool ParseValue<double>(const std::string& str, double& returnVal)
	{
		return ParseValue<double>(str, returnVal, [](const char* strValue, char** ret){
			return strtod(strValue, ret);
		});
	}
	
	template<>
	inline bool ParseValue<bool>(const std::string& str, bool& returnVal)
	{
		return ParseValue<bool>(str, returnVal, [](const char* strValue, char** ret){
			if(strcmp(strValue, "true") == 0)
			{
				*ret = "";
				return true;
			}
			if(strcmp(strValue, "false") == 0)
			{
				*ret = "";
				return false;
			}
			*ret = "0";
			return false;
		});
	}

	template<>
	inline bool ParseValue<std::string>(const std::string& str, std::string& returnVal)
	{
		return ParseValue<std::string>(str, returnVal, [](const char* strValue, char** ret){
			*ret = "";
			return strValue;
		});
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