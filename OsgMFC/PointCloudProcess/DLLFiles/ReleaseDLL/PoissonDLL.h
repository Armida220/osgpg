// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 POISSONDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// POISSONDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef POISSONDLL_EXPORTS
#define POISSONDLL_API __declspec(dllexport)
#else
#define POISSONDLL_API __declspec(dllimport)
#endif

#include "TriStruct.h"

POISSONDLL_API int PoissonReconFile(char *inputfile,char *outputfile);

POISSONDLL_API int PoissonReconBuffer(char *intputfile,typeMesh &trimesh);
