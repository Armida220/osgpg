#include <iostream>

#include "PoissonDLL.h"


using namespace std;

void OutputMesh(typeMesh &mesh,char *filename)
{
	FILE *fp = fopen(filename,"w");
	fprintf(fp,"%d %d \n",mesh.PointNum,mesh.TriNum);

	for (int i=0;i<mesh.PointNum;++i)
	{
		fprintf(fp,"%lf %lf %lf\n",mesh.Points[i].x,mesh.Points[i].y,mesh.Points[i].z);
	}

	for (int i=0;i<mesh.TriNum;++i)
	{
		fprintf(fp,"%lf %lf %lf\n",mesh.Triangles[i].pIndex1,mesh.Triangles[i].pIndex2,mesh.Triangles[i].pIndex3);
	}

}

int main()
{

#if 0
	PoissonReconFile("in.txt","out.ply");
#else
	typeMesh trimesh;
	PoissonReconBuffer("in.txt",trimesh);

	OutputMesh(trimesh,"out.ply");
#endif

	return 0;
}
