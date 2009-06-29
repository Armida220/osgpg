// osgViewerFramework.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "osgViewerFramework.h"

using namespace std;
using namespace osg;
using namespace osgViewer;

namespace FC {
	class ViewerFrameworkImpl
	{
	public:
		osg::ref_ptr<osgViewer::Viewer>				Viewer;
		osg::ref_ptr<osg::GraphicsContext::Traits>	Traits;
		osg::ref_ptr<osg::GraphicsContext>			Gc;
		osg::ref_ptr<osg::Camera>					Camera;
		osg::ref_ptr<osg::Group>					Root;
		HANDLE										Thread;

		bool Closed_yet;

	public:
		explicit ViewerFrameworkImpl(void) 
			: Viewer(new osgViewer::Viewer),
			Root(new osg::Group),
			Closed_yet(false)
		{}

		enum { SLEEP_TIME = 1000 };

		int SetRoot(osg::Node* node)
		{
			if ( node )
			{
				Root->addChild(node);
			}
			else
			{
				throw std::exception(
					"osgDB::readNodeFile() Failed "
					"in ViewerFramework::Load()\n");
			}

			return 0;
		}

		void Close()
		{
			Viewer->setDone(true);
			::WaitForSingleObject(this->Thread, INFINITE);
			Viewer->stopThreading();
			Closed_yet = true;
		}

		~ViewerFrameworkImpl()
		{
			if ( !Closed_yet )
			{
				Close();
			}
		}
	};

	ViewerFramework::ViewerFramework(void) : m_impl(new ViewerFrameworkImpl)
	{
	}

	ViewerFramework::~ViewerFramework(void)
	{
		if(m_impl)
			ReleaseImpl();
	}

	struct SetupCamera
	{
		void operator() (ViewerFrameworkImpl* pick, HWND hwnd, RECT rect)
		{
			// Init the GraphicsContext Traits
			pick->Traits = new osg::GraphicsContext::Traits;
			osg::GraphicsContext::Traits* traits = pick->Traits;

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
			pick->Gc = osg::GraphicsContext::createGraphicsContext(traits);
			osg::GraphicsContext* gc = pick->Gc;

			// Init a new Camera (Master for this View)
			osg::Camera* camera = new osg::Camera;

			// Assign Graphics Context to the Camera
			camera->setGraphicsContext(gc);

			//FC
			//set clear color as deep blue 
			camera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));

			// Set the viewport for the Camera
			camera->setViewport(new osg::Viewport(traits->x, traits->y, traits->width, traits->height));
			camera->setProjectionMatrixAsPerspective( 30.0f, (double)traits->width/(double)traits->height, 1.0f, 10000.0f );

			// Add the Camera to the Viewer
			pick->Viewer->setCamera(camera);
		}
	};

	struct SetupManipulatorAndEventHandler
	{
		void operator() (ViewerFrameworkImpl* pick)
		{
			// set up the camera manipulators.
			osgGA::KeySwitchMatrixManipulator* keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

			keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
			keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );

			osgViewer::Viewer* viewer = pick->Viewer;

			viewer->setCameraManipulator( keyswitchManipulator );

			// add the state manipulator
			viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );

			// add the thread model handler
			viewer->addEventHandler(new osgViewer::ThreadingHandler);

			// add the window size toggle handler
			viewer->addEventHandler(new osgViewer::WindowSizeHandler);

			// add the stats handler
			viewer->addEventHandler(new osgViewer::StatsHandler);

			// add the record camera path handler
			viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);

			// add the LOD Scale handler
			viewer->addEventHandler(new osgViewer::LODScaleHandler);
		}
	};

	struct StartRendering
	{
		void operator() (ViewerFrameworkImpl* pick)
		{
			pick->Closed_yet = false;
			pick->Viewer->setSceneData((osg::Node*) pick->Root.get());
			pick->Viewer->realize();
			pick->Thread = (HANDLE) ::_beginthread(&StartRendering::DoRendering, 0, (void*) pick->Viewer);
		}

		static void DoRendering(void* data)
		{
			osg::ref_ptr<osgViewer::Viewer> viewer = (osgViewer::Viewer*) data;

			while ( !viewer->done() )
			{
				viewer->frame();
			}

			::_endthread();
		}
	};

	void ViewerFramework::SetupWindow(HWND hwnd, RECT rect)
	{
		SetupCamera()(m_impl, hwnd, rect);
		SetupManipulatorAndEventHandler()(m_impl);
	}

	bool ViewerFramework::Load(string path)
	{
		setlocale(LC_ALL, "chs");
		osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(path);
		if(node.valid()) {
			m_impl->SetRoot(node);
			return true;
		}
		return false;
	}

	void ViewerFramework::Load(osg::Node* node)
	{
		m_impl->SetRoot(node);
	}

	void ViewerFramework::Run()
	{
		StartRendering()(m_impl);
	}

	void ViewerFramework::Stop()
	{
		m_impl->Close();
		//delete m_impl;
	}

	void ViewerFramework::ReleaseImpl()
	{
		delete m_impl;
		m_impl = 0;
	}

	void ViewerFramework::AddEventHandler(osgGA::GUIEventHandler* handler)
	{
		m_impl->Viewer->addEventHandler(handler);
	}

	osgViewer::Viewer* ViewerFramework::GetViewer()
	{
		return m_impl->Viewer.get();
	}
}

