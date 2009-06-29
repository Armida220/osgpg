#pragma once
#include <osg/Node>
#include <osgGA/GUIEventHandler>
#include <vector>
using namespace std;

namespace FC {
	//调用者相关信息，目前主要用于存储UI信息
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

	//凡继承自WorkFlowPluginBase的必须各自实现这个函数
	//bool CreateWorkFlowPlugin(void ** pobj);
	//{
	//	*pobj = new CPlugA;
	//	return *pobj != NULL;
	//}
}