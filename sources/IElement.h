#pragma once

class INodeList;

class IElement
{
public:
	IElement(){};
	IElement(const IElement& other){};
	virtual ~IElement(){};
	
	virtual const char* GetType()	{ return "None"; };
	
	virtual void AssignElasticityMatrix(const Eigen::MatrixXd& D) = 0;
	virtual bool PrecomputeAndValidateGeometry() = 0;
};
