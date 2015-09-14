#pragma once
#include <vector>

class INodeList;

class IElement
{
	IElement(const IElement& other){};
public:
	IElement(){};
	virtual ~IElement(){};

	virtual const char* GetType()	{ return "None"; };
	virtual void SetIndices(const std::vector<int>& indices) = 0;
	virtual std::vector<int> GetIndices() const = 0;
};
