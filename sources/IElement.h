#pragma once

class INodeList;

class IElement
{
	IElement(const IElement& other){};
public:
	IElement(){};
	virtual ~IElement(){};

	virtual const char* GetType()	{ return "None"; };
};

class Linear2DElement
{
public:
	virtual const char* GetType()	{ return "Linear2DElement"; };

private:
    float   m_K[36];
    int	    m_nodes[3];
    float   m_area;
};

class Elements
{
public:
	void Init(int nodeCount);
	float& operator ()(int i);

private:
	int m_elementsCount;
	IElement** m_data;
};

inline void Elements::Init(int elementsCount)
{
    m_elementsCount = elementsCount;
    m_data = new IElement*[m_elementsCount];
}

inline float& Elements::operator()(int i)
{
    return * (m_data + i);
}
