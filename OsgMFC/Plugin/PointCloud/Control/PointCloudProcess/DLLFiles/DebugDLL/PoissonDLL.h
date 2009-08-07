// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� POISSONDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// POISSONDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef POISSONDLL_EXPORTS
#define POISSONDLL_API __declspec(dllexport)
#else
#define POISSONDLL_API __declspec(dllimport)
#endif

#include "TriStruct.h"

POISSONDLL_API int PoissonReconFile(char *inputfile,char *outputfile);

POISSONDLL_API int PoissonReconBuffer(char *intputfile,typeMesh &trimesh);
