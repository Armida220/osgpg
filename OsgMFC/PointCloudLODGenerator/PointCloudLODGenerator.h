// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� POINTCLOUDLODGENERATOR_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// POINTCLOUDLODGENERATOR_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef POINTCLOUDLODGENERATOR_EXPORTS
#define POINTCLOUDLODGENERATOR_API __declspec(dllexport)
#else
#define POINTCLOUDLODGENERATOR_API __declspec(dllimport)
#endif

#include "WorkFlowPluginBase.h"
using namespace std;

namespace FC {
	class POINTCLOUDLODGENERATOR_API PointCloudLODGenerator :
		public WorkFlowPluginBase{
	protected:
		osg::ref_ptr<osg::Node> sceneRoot;
	public:
		virtual osg::Node* CreateSceneData();
		virtual void Release();
	};
}

extern "C" {
	POINTCLOUDLODGENERATOR_API bool CreateWorkFlowPlugin(void ** pobj);
}