#include "XMLDataUtils.h"
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>

xmldata::TiXmlDocumentPtr xmldata::OpenXMLDoc(std::string filename)
{
	std::ifstream stream;
	stream.open(filename.c_str());
	std::string str;

	stream.seekg(0, std::ios::end);
    str.reserve(stream.tellg());
    stream.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(stream)),
    std::istreambuf_iterator<char>());

    TiXmlDocumentPtr xml(new TiXmlDocument());
	xml->Parse(str.c_str(), 0, TIXML_ENCODING_UTF8);

	return TiXmlDocumentPtr(xml);
}

TiXmlNode* xmldata::GetNodeByPath(TiXmlNode* node, const std::string& path)
{
	std::vector<std::string> pathSplitted;
	typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep("./");
	tokenizer tokens(path, sep);
	TiXmlNode* nodeCurrent = node;
	for(tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
		nodeCurrent = nodeCurrent->FirstChild(*tok_iter);
		if(nodeCurrent == NULL)
		{
			return NULL;
		}
	}
	return nodeCurrent;
}

std::string xmldata::GetNodeText(TiXmlNode* node, const std::string& path)
{
	TiXmlNode* nodeOfInterest = xmldata::GetNodeByPath(node, path);
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
