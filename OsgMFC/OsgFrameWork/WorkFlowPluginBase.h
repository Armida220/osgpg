#pragma once
#include <osg/Node>
#include <osgGA/GUIEventHandler>
#include <vector>
using namespace std;

namespace FC {
	//�����������Ϣ��Ŀǰ��Ҫ���ڴ洢UI��Ϣ
	typedef void *LauncherInfo;

	typedef vector<osgGA::GUIEventHandler*> GUIEventHandlerArr;

	class WorkFlowPluginBase
	{
	protected:
		WorkFlowPluginBase() { _launcherInfo = 0; }
		virtual ~WorkFlowPluginBase() {}
		LauncherInfo _launcherInfo;

	public:
		virtual osg::Node* CreateSceneData() = 0;
		virtual void Release() { delete this; }
		virtual void SetLauncherInfo(LauncherInfo launcherInfo) { _launcherInfo = launcherInfo; }
		virtual GUIEventHandlerArr CreateGUIEventHandlerArr() { GUIEventHandlerArr arr; return arr; }
	};

	//���̳���WorkFlowPluginBase�ı������ʵ���������
	//bool CreateWorkFlowPlugin(void ** pobj);
	//{
	//	*pobj = new CPlugA;
	//	return *pobj != NULL;
	//}
}