#pragma once
#include <osg/Node>

namespace FC {
	//�����������Ϣ��Ŀǰ��Ҫ���ڴ洢UI��Ϣ
	typedef void *LauncherInfo;

	class WorkFlowPluginBase
	{
	protected:
		WorkFlowPluginBase() { _launcherInfo = 0; }
		virtual ~WorkFlowPluginBase() {}
		LauncherInfo _launcherInfo;

	public:
		virtual osg::Node* CreateSceneData() = 0;
		virtual void Release() = 0;
		virtual void SetLauncherInfo(LauncherInfo launcherInfo) { _launcherInfo = launcherInfo; }
	};

	//���̳���WorkFlowPluginBase�ı������ʵ���������
	//bool CreateWorkFlowPlugin(void ** pobj);
	//{
	//	*pobj = new CPlugA;
	//	return *pobj != NULL;
	//}
}