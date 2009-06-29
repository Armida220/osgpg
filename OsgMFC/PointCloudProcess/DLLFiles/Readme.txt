1、将Poisson.h 和TriStruct.h两个头文件加入你的工程中。

2、DLL的导出函数：
//inputfile为输入数据文件的路径，outputfile为构网后的输出文件的路径。
POISSONDLL_API int PoissonReconFile(char *inputfile,char *outputfile);

//inputfile为输入数据文件的路径，trimesh为构网后记录点云组织方式的结构体，包括点的坐标，三角形的顶点序号等信息，详见TriStruct.h 。
POISSONDLL_API int PoissonReconBuffer(char *intputfile,typeMesh &trimesh);

3、使用方式可参考Sample.h。