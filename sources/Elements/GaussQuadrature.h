#pragma once

class GaussQuadrature
{
public:

	template<int X>
	static bool GetWeights(int i, float& a, float& w);
	
#define GAUSS_QUADRATURE( X ) \
private:\
	static float k_G##X##_AbsTable[X+1];\
	static float k_G##X##_WeightTable[X+1];\
public:\
	template<>\
	static bool GetWeights< X >(int i, float& a, float& w)\
	{\
		a = k_G##X##_AbsTable[i];\
		w = k_G##X##_WeightTable[i];\
		return i < X;\
	};\
private:

	GAUSS_QUADRATURE(2);
	GAUSS_QUADRATURE(3);
	GAUSS_QUADRATURE(4);
	GAUSS_QUADRATURE(5);
	GAUSS_QUADRATURE(6);
	GAUSS_QUADRATURE(7);
	GAUSS_QUADRATURE(10);

#undef GAUSS_QUADRATURE
};

