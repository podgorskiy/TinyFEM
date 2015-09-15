#pragma once
#include <vector>

struct NodalForce
{
	float x, y, z;
	int node;
};

typedef std::vector<NodalForce> NodalForceList;

struct NodalConstraint
{
	enum ConstraintType
	{
		UX = 1,
		UY = 1 << 1,
		UXY = UX | UY
	};
	ConstraintType type;
	int node;
};

typedef std::vector<NodalConstraint> NodalConstraintList;