#include <stdio.h>
#include <stdlib.h>
#include "plyreader.h"

FILE *dumpfp=0;
static int vertex_cb(p_ply_argument argument) {
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    fprintf(dumpfp,"%lf", ply_get_argument_value(argument));
    if (eol) fprintf(dumpfp,"\n");
    else fprintf(dumpfp," ");
    return 1;
}

static int face_cb(p_ply_argument argument) {
    long length, value_index;
    ply_get_argument_property(argument, NULL, &length, &value_index);
    switch (value_index) {
        case 0:
        case 1: 
            fprintf(dumpfp,"%g ", ply_get_argument_value(argument));
            break;
        case 2:
            fprintf(dumpfp,"%g\n", ply_get_argument_value(argument));
            break;
        default: 
            break;
    }
    return 1;
}

int PlyRead(char *filename, typeMesh &trimesh){
    long i,nvertices, ntriangles;
    p_ply ply = ply_open(filename, NULL);
    if (!ply) return 1;

	dumpfp = fopen("__tmp__","w");

    if (!ply_read_header(ply)) return 1;
    nvertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);
    ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);
    //fprintf(dumpfp,"%ld\n%ld\n", nvertices, ntriangles);
    if (!ply_read(ply)) return 1;
    ply_close(ply);
	fclose(dumpfp);
	
	dumpfp = fopen("__tmp__","r");
	typePoint3D tmpPoint3D;
	for ( i=0;i<nvertices;++i)
	{
		fscanf(dumpfp,"%lf%lf%lf",&tmpPoint3D.x,&tmpPoint3D.y,&tmpPoint3D.z);
		trimesh.Points.push_back(tmpPoint3D);
	}
	typeTri tmpTri;
	for ( i=0;i<ntriangles;++i)
	{
		fscanf(dumpfp,"%d%d%d",&tmpTri.pIndex1,&tmpTri.pIndex2,&tmpTri.pIndex3);
		trimesh.Triangles.push_back(tmpTri);
	}

	trimesh.PointNum = trimesh.Points.size();
	trimesh.TriNum = trimesh.Triangles.size();

	fclose(dumpfp);
	system("del __tmp__");
    return 0;
}

/*int main()
{
	typeMesh trimesh;
	PlyRead("texturedknot.ply",trimesh);
	return 0;
}*/
