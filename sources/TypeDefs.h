#pragma once

typedef int INodeID;
typedef int IShapeID;
typedef int INodeLID;

namespace fem
{
	enum ProblemType
	{
		PT_FlatStress,
		PT_FlatStraing,
		PT_VolumetricStressStrain
	};
	enum ProblemDimension
	{
		PD_TwoDimensional,
		PD_ThreeDimensional,
	};
	enum GaussIntegration
	{
		GI_3X3,
		GI_4X4,
		GI_5X5
	};
}