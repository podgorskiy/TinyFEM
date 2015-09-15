#pragma once
#include <string>
#include "StrideDataFixedArray.h"
#include "Elements/ElementsContainer.h"

class ProblemDescription
{

};

class TiXmlDocument;

class Problem
{
	Problem(const Problem& other){};
	void operator=(const Problem& other){};
public:
	Problem();
	~Problem();

	void OpenFromMemory(const std::string& memoryString);
	void OpenFromFile(const std::string& filename);

	void SaveToFile(const std::string& filename) const;
	const ProblemDescription& GetProblemDescription() const;
	const StrideDataFixedArray& GetNodes() const;
	const ElementsContainer& GetElements() const;
private:
	void OpenDoc(TiXmlDocument* hDoc);

	StrideDataFixedArray m_nodes;
	ElementsContainer m_elements;
	ProblemDescription m_desc;
};
