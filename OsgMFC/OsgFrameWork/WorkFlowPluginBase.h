#pragma once
#include <osg/Node>
#include <osgGA/GUIEventHandler>
#include <vector>
using namespace std;

namespace FC {
	typedef vector<osgGA::GUIEventHandler*> GUIEventHandlerArr;

	class WorkFlowPluginBase
	{
	protected:
		WorkFlowPluginBase() { }
		virtual ~WorkFlowPluginBase() {}

	public:
		virtual osg::Node* CreateSceneData() = 0;
		virtual void Release() { delete this; }
		virtual GUIEventHandlerArr CreateGUIEventHandlerArr() { GUIEventHandlerArr arr; return arr; }
	};

	//���̳���WorkFlowPluginBase�ı������ʵ���������
	//bool CreateWorkFlowPlugin(void ** pobj);
	//{
	//	*pobj = new CPlugA;
	//	return *pobj != NULL;
	//}
}