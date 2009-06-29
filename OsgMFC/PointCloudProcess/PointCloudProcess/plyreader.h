#ifndef __PLYREADER__
#define __PLYREADER__ 1

#include <stdio.h>
#include <stdlib.h>
#include "rply.h"
#include "TriStruct.h"

extern FILE *dumpfp;
static int vertex_cb(p_ply_argument argument);
static int face_cb(p_ply_argument argument);
int PlyRead(char *filename, typeMesh &trimesh);

/*int main()
{
	typeMesh trimesh;
	PlyRead("texturedknot.ply",trimesh);
	return 0;
}*/
#endif