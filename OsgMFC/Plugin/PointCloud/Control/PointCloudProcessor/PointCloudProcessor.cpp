// PointCloudProcessor.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../PointCloudProcess/PointCloudProcess/CloudStation.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
#include "../PointCloudProcess/PointCloudProcess/DataLoader.h"
#include "../PointCloudProcess/PointCloudProcess/Utility.h"

#include "PointCloudProcessor.h"
#include "PointCloudOpener.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;
//////////////////////////////////////////////////////////////////////////

bool CreateWorkFlowPlugin(void **pobj)
{
	*pobj = new FC::PointCloudProcessor;
	return *pobj != NULL;
}

namespace FC {
	void PointCloudProcessor::Release()
	{
#define SAFE_RELEASE(name) if(name) { name->DestroyWindow(); delete name; name = 0; }
		SAFE_RELEASE(setter)
		SAFE_RELEASE(viewDlg)
		SAFE_RELEASE(lableDlg)
		SAFE_RELEASE(pickPointDlg)
		SAFE_RELEASE(pickTriangleDlg)
		SAFE_RELEASE(dragDlg)
		SAFE_RELEASE(dragVertexDlg)
		SAFE_RELEASE(imageOpenerDlg)

		delete this;
	}

	void PointCloudProcessor::SetSceneData(osgViewer::CompositeViewer* viewer)
	{
		osg::ref_ptr<osg::Group> ret = new osg::Group;
		this->sceneRoot = ret.get();

		osg::Group* root = viewer->getView(0)->getSceneData()->asGroup();
		root->addChild(ret);
        osg::Viewport* vport = viewer->getView(0)->getCamera()->getViewport();
        root->addChild(Utility::createGlobalHud(vport->width(),vport->height()));

		setlocale(LC_ALL, "chs");

		AfxMessageBox("��һ����ѡ�����/�������ļ������Դ򿪶���ļ����ԡ�ȡ��������");
//#ifndef _DEBUG
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		PointCloudOpener pcOpener;

		PointCloudReadWriter cptRW;
		TriMeshReadWriter tmtRW;

		while(pcOpener.DoModal()==IDOK) {
			if(pcOpener.m_iPointCloudChoice!=0) {
				osg::ref_ptr<CloudStation> cs = cptRW.Read(pcOpener.m_strFileName.GetBuffer(), 
					pcOpener.m_iPointCloudChoice-1);
				ret->addChild(cs.get());
			} else {
				osg::ref_ptr<CloudStation> cs2 = tmtRW.Read(pcOpener.m_strFileName.GetBuffer(), 
					pcOpener.m_iTriangleChoice-1);
				ret->addChild(cs2.get());
			}
		}
//#endif

		AfxMessageBox("�ڶ�������������ݵ���/������");
	}

	void PointCloudProcessor::SetGUIEventHandlers(osgViewer::CompositeViewer* viewer)
	{	
		osg::ref_ptr<Controller> ctrl = new Controller;
		viewer->getView(0)->addEventHandler(ctrl);

		AfxMessageBox("������������");

//#ifndef _DEBUG
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		setter = new CPropertySheet("���ƿ�����");
		
		viewDlg = new ViewModeDialog;
		viewDlg->ctrl = ctrl.get();
		setter->AddPage(viewDlg);

		lableDlg = new LablePointModeDialog;
		lableDlg->ctrl = ctrl.get();
		setter->AddPage(lableDlg);

		pickPointDlg = new PickPointCloudModeDialog;
		pickPointDlg->ctrl = ctrl.get();
		setter->AddPage(pickPointDlg);

		pickTriangleDlg = new PickTriangleModeDialog;
		pickTriangleDlg->ctrl = ctrl.get();
		setter->AddPage(pickTriangleDlg);

		dragDlg = new DragModeDialog;
		dragDlg->ctrl = ctrl.get();
		setter->AddPage(dragDlg);

		dragVertexDlg = new DragVertexModeDialog;
		dragVertexDlg->ctrl = ctrl.get();
		setter->AddPage(dragVertexDlg);

		imageOpenerDlg = new ImageOpener;
		setter->AddPage(imageOpenerDlg);

		setter->Create();
		setter->ShowWindow(SW_SHOW);
//#endif
	}
}