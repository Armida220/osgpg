#pragma once
#include <osg/Node>
#include <osgGA/GUIEventHandler>
#include <vector>
#include <map>
using namespace std;

namespace FC {
	typedef osgGA::GUIEventHandler Controller;

	class ControllerPluginBase
	{
	protected:
		ControllerPluginBase() { _controller = 0; }
		virtual ~ControllerPluginBase() {}
		osg::ref_ptr<Controller> _controller;

	public:
		virtual Controller* CreateController() = 0;
		virtual void Release() { delete this; }
	};
}

//凡继承自ControllerPluginBase的必须各自实现这个函数
//bool CreateControllerPlugin(void ** pobj);
//{
//	*pobj = new CPlugA;
//	return *pobj != NULL;
//}