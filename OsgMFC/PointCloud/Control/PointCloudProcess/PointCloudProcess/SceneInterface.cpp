#include "StdAfx.h"
#include "SceneInterface.h"
#include "process.h"
#include "osgViewer/api/Win32/GraphicsWindowWin32"
#include <osgDB/WriteFile>

SceneInterface::SceneInterface(void)
{
	m_initialized = false;
	m_done = false;
	m_Viewer = new osgViewer::Viewer;

	//FC {
	ctrl = new Controller;
	//FC }
	
}

SceneInterface::~SceneInterface(void)
{
//	if(m_Viewer.valid() && m_initialized )
//	{
//	}
}
void SceneInterface::InitViewer(int x, int y, int width, int height, HWND hwnd)
{
	//如果初始化了 就不再初始话
	if(!m_initialized)
	{
		//用osg::GraphicsContext::Traits结构体保存 要创建的窗体信息
		osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(hwnd);
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = x;
		traits->y = y;
		traits->width = width;
		traits->height = height;
		traits->windowDecoration = false;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->inheritedWindowData = windata;
		traits->setInheritedWindowPixelFormat = true;
		osg::ref_ptr<osg::GraphicsContext> graphics = osg::GraphicsContext::createGraphicsContext(traits.get());

		//创建合适的投影 防止窗口创建时候图形的变形
		osg::ref_ptr<osg::Camera> camera = m_Viewer->getCamera();
		double fovy , aspectRatio , zNear , zFar , newaspectRatio;
		camera->getProjectionMatrixAsPerspective(fovy,aspectRatio,zNear,zFar);
		newaspectRatio = (double)traits->width/(double)traits->height;
		camera->getProjectionMatrix() *= osg::Matrix::scale(aspectRatio/newaspectRatio,1.0,1.0);
		
		//FC {
		m_Viewer->addEventHandler(ctrl.get());
		//FC }
		m_Viewer->getCamera()->setGraphicsContext( graphics.get() );
		m_Viewer->getCamera()->setViewport(new osg::Viewport(traits->x,traits->y,traits->width,traits->height));
		m_Viewer->getCamera()->setDrawBuffer(GL_BACK);
		m_Viewer->getCamera()->setReadBuffer(GL_BACK);
		m_Viewer->realize();
		m_initialized = true;
	}
}

void SceneInterface::SetSceneData(std::string str)
{
	if(m_Viewer->getSceneData() == NULL)
	{
		osg::ref_ptr<osg::Group> root = new osg::Group;
		osg::Node *node = new osg::Node;
		node = osgDB::readNodeFile(str);
		root->addChild( node );
		m_Viewer->setSceneData(root.get());
		if( root.valid() )
		{
			m_Viewer->updateSlaves();
			m_Viewer->frame();
			osgGA::TrackballManipulator *trackball = new osgGA::TrackballManipulator; 
			osgGA::KeySwitchMatrixManipulator *keySwitchMatrixManipulator = new osgGA::KeySwitchMatrixManipulator;
			keySwitchMatrixManipulator->addMatrixManipulator('1',"Trackball",trackball);
			keySwitchMatrixManipulator->selectMatrixManipulator(0);
			m_Viewer->setCameraManipulator(keySwitchMatrixManipulator);
		}
	}
}

bool SceneInterface::DoUpdate()
{
	if(m_initialized && m_Viewer.valid() && (m_Viewer->getSceneData() != NULL))
	{
		m_Viewer->updateSlaves();
		m_Viewer->frame();
		return true;
	}
	else
	{
		osg::notify(osg::NOTICE)<<"m_Viewer bu ke yong"<<std::endl;
		return false;
	}
}