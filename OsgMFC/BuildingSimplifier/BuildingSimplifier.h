// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� BUILDINGSIMPLIFIER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// BUILDINGSIMPLIFIER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef BUILDINGSIMPLIFIER_EXPORTS
#define BUILDINGSIMPLIFIER_API __declspec(dllexport)
#else
#define BUILDINGSIMPLIFIER_API __declspec(dllimport)
#endif

#include "WorkFlowPluginBase.h"
using namespace std;

namespace FC {
	class BUILDINGSIMPLIFIER_API BuildingSimplifier  :
		public WorkFlowPluginBase{
	protected:
		osg::ref_ptr<osg::Node> sceneRoot;
	public:
		virtual osg::Node* CreateSceneData();
		virtual void Release();
	};
}

extern "C" {
	BUILDINGSIMPLIFIER_API bool CreateWorkFlowPlugin(void ** pobj);
}