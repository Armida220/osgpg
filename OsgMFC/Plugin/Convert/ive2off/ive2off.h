// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� IVE2OFF_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// IVE2OFF_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef IVE2OFF_EXPORTS
#define IVE2OFF_API __declspec(dllexport)
#else
#define IVE2OFF_API __declspec(dllimport)
#endif

#include "WorkFlowPluginBase.h"
using namespace std;

namespace FC {
	class IVE2OFF_API Ive2off :
		public WorkFlowPluginBase{
	protected:
		osg::ref_ptr<osg::Node> sceneRoot;
	public:
		virtual osg::Node* CreateSceneData();
		virtual void Release();
	};

}

extern "C" {
	IVE2OFF_API bool CreateWorkFlowPlugin(void ** pobj);
}