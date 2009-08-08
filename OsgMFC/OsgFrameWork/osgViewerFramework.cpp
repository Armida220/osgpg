// osgViewerFramework.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "osgViewerFramework.h"
#include "ViewerFrameworkImpl.h"

using namespace std;
using namespace osg;
using namespace osgViewer;

namespace FC {
	ViewerFramework::ViewerFramework(void) : m_impl(new ViewerFrameworkImpl)
	{
	}

	ViewerFramework::~ViewerFramework(void)
	{
		ReleaseImpl();
	}

	void ViewerFramework::SetupWindow(HWND hwnd, RECT rect)
	{
		m_impl->SetupCamera(hwnd, rect);
		m_impl->SetupManipulatorAndEventHandler();
	}

	void ViewerFramework::Resize(RECT rect)
	{
		m_impl->Resize(rect);
	}

	void ViewerFramework::InitSceneRoot()
	{
		m_impl->InitSceneRoot();
	}

	osg::Node* ViewerFramework::GetRoot(int i)
	{
		return m_impl->GetRoot(i);
	}

	bool ViewerFramework::AddModel(std::string path, int i)
	{
		osg::Node* model = osgDB::readNodeFile(path);
		if(model) {
			m_impl->AddModel(model, i);
			return true;
		}
		return false;
	}

	void ViewerFramework::AddModel(osg::Node* node, int i)
	{
		m_impl->AddModel(node, i);
	}

	void ViewerFramework::Run()
	{
		m_impl->StartRendering();
	}

	void ViewerFramework::Stop()
	{
		m_impl->StopRendering();
	}

	void ViewerFramework::AddEventHandler(osgGA::GUIEventHandler* handler, int i)
	{
		m_impl->Viewer->getView(i)->addEventHandler(handler);
	}

	osgViewer::CompositeViewer* ViewerFramework::GetViewer()
	{
		return m_impl->Viewer.get();
	}
	void ViewerFramework::ResetImpl(FC::ViewerFrameworkImpl *newImpl)
	{
		if(newImpl) {
			ReleaseImpl(); //release impl now
			this->m_impl = newImpl;
		}
	}
	void ViewerFramework::ReleaseImpl()
	{
		if(m_impl)
			m_impl->Release();
		m_impl = 0;
	}
}

