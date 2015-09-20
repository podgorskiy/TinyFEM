#pragma once
#include <vector>

struct NodalConstraint
{
	enum ConstraintType
	{
		UX = 1,
		UY = 1 << 1,
		UZ = 1 << 2,
		UXY = UX | UY,
		UYZ = UY | UZ,
		UZX = UZ | UX,
		UXYZ = UX | UY |UZ,
	};
	ConstraintType type;
	int node;
};

typedef std::vector<NodalConstraint> NodalConstraintList;