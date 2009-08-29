// ViewerFrameworkImpl.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "ViewerFrameworkImpl.h"


#include <osg/NodeVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

using namespace std;
using namespace osg;
using namespace osgViewer;
using namespace FC;

ViewerFrameworkImpl::ViewerFrameworkImpl(void) : Viewer(new osgViewer::CompositeViewer()),
Root(new osg::Group), Closed_yet(false)
{}
ViewerFrameworkImpl::~ViewerFrameworkImpl()
{
	if ( !Closed_yet )
	{
		StopRendering();
	}
}

osg::Node* ViewerFrameworkImpl::GetOrCreateRoot(int i/* =0 */)
{
	if(i>=Viewer->getNumViews()) {
		throw std::exception(
			"View number is less than i"
			"in ViewerFrameworkImpl::GetOrCreateRoot(int i)\n");
		return 0;
	}

	if(i==0) {
		Viewer->getView(0)->setSceneData(Root);
		return Root;
	}

	osg::Node* node = Viewer->getView(i)->getSceneData();
	if(node==0) {
		node = new osg::Group;
		char name[1024];
		sprintf(name, "Root%i", i);
		node->setName(name);
		GetOrCreateRoot(0)->asGroup()->addChild(node);
		Viewer->getView(i)->setSceneData(node);
	}
	return node;
}

osg::Node* ViewerFrameworkImpl::GetRoot(int i)
{
	if(i>=Viewer->getNumViews()) {
		throw std::exception(
			"View number is less than i"
			"in ViewerFrameworkImpl::GetRoot(int i)\n");
		return 0;
	}
	return Viewer->getView(i)->getSceneData();
}

bool ViewerFrameworkImpl::AddModel(osg::Node* node, int i)
{
	if (node)	{
		return GetOrCreateRoot(i)->asGroup()->addChild( node );
	}	else {
		throw std::exception(
			"node is NULL"
			"in ViewerFrameworkImpl::AddModel(osg::Node* node)\n");
		return false;
	}
}

void ViewerFrameworkImpl::Resize(RECT rect)
{
}

void ViewerFrameworkImpl::SetupCamera(HWND hwnd, RECT rect)
{
	// Init the GraphicsContext Traits
	Traits = new osg::GraphicsContext::Traits;
	osg::GraphicsContext::Traits* traits = Traits;

	// Init the Windata Variable that holds the handle for the Window to display OSG in.
	osg::Referenced* windata = new osgViewer::GraphicsWindowWin32::WindowData(hwnd);

	// Setup the traits parameters
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;

	// Create the Graphics Context
	Gc = osg::GraphicsContext::createGraphicsContext(traits);
	osg::GraphicsContext* gc = Gc;

	osgViewer::View* view = new osgViewer::View;
	view->setName("Single view");
	Viewer->addView(view);

	// Init a new Camera (Master for this View)
	osg::Camera* camera = new osg::Camera;

	// Assign Graphics Context to the Camera
	camera->setGraphicsContext(gc);

	//set clear color as deep blue 
	camera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));

	// Set the viewport for the Camera
	camera->setViewport(new osg::Viewport(traits->x, traits->y, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective( 30.0f, (double)traits->width/(double)traits->height, 1.0f, 10000.0f );

	// Add the Camera to the view
	view->setCamera(camera);
}

void ViewerFrameworkImpl::SetupManipulatorAndEventHandler()
{
	// set up the camera manipulators.
	osgGA::KeySwitchMatrixManipulator* keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

	keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
	keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );

	//default view is view 0
	osgViewer::View* view = Viewer->getView(0);

	view->setCameraManipulator( keyswitchManipulator );

	// add the state manipulator
	view->addEventHandler( new osgGA::StateSetManipulator(view->getCamera()->getOrCreateStateSet()) );

	// add the thread model handler
	view->addEventHandler(new osgViewer::ThreadingHandler);

	// add the window size toggle handler
	view->addEventHandler(new osgViewer::WindowSizeHandler);

	// add the stats handler
	view->addEventHandler(new osgViewer::StatsHandler);

	// add the record camera path handler
	view->addEventHandler(new osgViewer::RecordCameraPathHandler);

	// add the LOD Scale handler
	view->addEventHandler(new osgViewer::LODScaleHandler);
}

void ViewerFrameworkImpl::InitSceneRoot()
{
	for(unsigned int i=0; i<Viewer->getNumViews(); ++i) {
		GetOrCreateRoot(i);
	}
}

void ViewerFrameworkImpl::StartRendering()
{
	Closed_yet = false;
	//default view : view 0
	//Viewer->getView(0)->setSceneData((osg::Node*) Root.get());
	Viewer->getView(0)->home();
	Viewer->realize();
	Thread = (HANDLE) ::_beginthread(&ViewerFrameworkImpl::DoRendering, 0, (void*)Viewer);
}

void ViewerFrameworkImpl::DoRendering(void* data)
{
	osg::ref_ptr<osgViewer::CompositeViewer> viewer = (osgViewer::CompositeViewer*) data;

	while ( !viewer->done() )
	{
		viewer->frame();
	}

	::_endthread();
}

void ViewerFrameworkImpl::StopRendering()
{
	Viewer->setDone(true);
	::WaitForSingleObject(this->Thread, INFINITE);
	Viewer->stopThreading();
	Closed_yet = true;
}

void ViewerFrameworkImpl::Release()
{
	delete this;
}