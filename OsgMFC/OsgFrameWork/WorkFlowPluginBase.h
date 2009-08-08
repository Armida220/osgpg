#pragma once
#include <osg/Node>
#include <osgGA/GUIEventHandler>
#include "ViewerFrameworkImpl.h"
#include "osgViewer/CompositeViewer"
#include <vector>
using namespace std;

namespace FC {
	class WorkFlowPluginBase
	{
	protected:
		WorkFlowPluginBase() {}
		virtual ~WorkFlowPluginBase() {}

	public:
		virtual void SetSceneData(osgViewer::CompositeViewer* viewer) = 0;
		virtual void SetGUIEventHandlers(osgViewer::CompositeViewer* viewer) {}
		virtual ViewerFrameworkImpl* CreateViewerFrameworkImpl() { return 0; }
		virtual void Release() { delete this; }
	};
}

//���̳���WorkFlowPluginBase�ı������ʵ���������
//bool CreateWorkFlowPlugin(void ** pobj);
//{
//	*pobj = new CPlugA;
//	return *pobj != NULL;
//}