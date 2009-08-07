/********************************************************************
	filename: 	Project\PointCloud\PointCloudProcess\SceneInterface.h
	file path:	Project\PointCloud\PointCloudProcess
	file base:	SceneInterface
	file ext:	h
	
	purpose:	ViewerInterface初始化OSG场景
*********************************************************************/
#pragma once
#include "export.h"
#include <osg/ref_ptr>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgUtil/Optimizer>
#include <osgViewer/ViewerEventHandlers>
//FC {
#include "Controller.h"
//FC }

class POINTCLOUDPROCESS_API SceneInterface : public osg::Referenced
{
public:
    SceneInterface(void);

    //创建OSG::viewer,设置照相机，事件处理器，相机控制器
    void InitViewer(int x,int y,int width,int height,HWND hwnd);

    bool DoUpdate(void);

    bool Done(void) { m_done = m_Viewer.get()->done(); return m_done;}

    void SetBackGroundColor(osg::Vec4 color) {}

    osg::Node* GetSceneData(void) { return 0;}

	void SetSceneData(std::string str);

	void ChangeViewerSize(int cx , int cy);

	bool m_initialized;

	//FC {
	Controller* GetController() { return ctrl.get(); }
	//FC }
protected:

	//FC {
	osg::ref_ptr<Controller> ctrl;
	//FC }
    bool m_done;

	 ~SceneInterface(void);

	osg::ref_ptr<osgViewer::Viewer> m_Viewer;	
};
