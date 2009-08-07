#ifndef TRISTRUCT_H_
#define TRISTRUCT_H_

#include <vector>
using namespace std;

typedef struct tagPoint3D
{
	double x;
	double y;
	double z;

}typePoint3D;

typedef struct  tagTriangle
{
	int pIndex1,pIndex2,pIndex3;

}typeTri;

class typeMesh
{
public:
	int PointNum,TriNum;	
	vector<typePoint3D> Points;
	vector<typeTri> Triangles;

	typeMesh(void){PointNum=TriNum=0;};
};

#endif