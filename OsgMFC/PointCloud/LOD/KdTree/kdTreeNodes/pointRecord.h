#pragma once

#include <list>
#include <vector>
#include <float.h>
using namespace std;

enum RecordType {
	XYZ=0,
	XYZI,
	XYZRGB,
	XYZIRGB
};

struct PointRec {
	float x,y,z;
	float i;
	float r,g,b;

	PointRec(float _x=0, float _y=0, float _z=0,
		float _i=-1, float _r=-1, float _g=-1, float _b=-1) {
			x=_x;	y=_y;	z=_z;
			i=_i;
			r=_r; g=_g; b=_b;
	}

	inline float operator[] (int i)
	{
		if(i==0)
			return x;
		else if(i==1)
			return y;
		else
			return z;
	}

	static float maxI, minI;
	static void Reset_I(float newMaxI=-FLT_MAX, float newMinI=FLT_MAX);
	static float GetMaxI();
	static float GetMinI();
	static void Update_I(float newI);
};

typedef vector<PointRec>		PointSet;
typedef unsigned int PointIdx;
typedef vector<PointIdx>	PointIdxSet;