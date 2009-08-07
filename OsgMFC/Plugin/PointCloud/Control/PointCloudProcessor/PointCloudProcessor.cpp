// PointCloudProcessor.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "PointCloudOpener.h"

#include "../PointCloudProcess/PointCloudProcess/CloudStation.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
#include "../PointCloudProcess/PointCloudProcess/DataLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 唯一的应用程序对象

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

		delete this;
	}

	osg::Node* PointCloudProcessor::CreateSceneData()
	{
		osg::ref_ptr<osg::Group> root = new osg::Group;
		this->sceneRoot = root.get();

		setlocale(LC_ALL, "chs");

		AfxMessageBox("第一步，选择点云/三角网文件，可以打开多个文件，以“取消”结束");
#ifndef _DEBUG
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		PointCloudOpener pcOpener;

		PointCloudReadWriter cptRW;
		TriMeshReadWriter tmtRW;

		while(pcOpener.DoModal()==IDOK) {
			if(pcOpener.m_iPointCloudChoice!=0) {
				osg::ref_ptr<CloudStation> cs = cptRW.Read(pcOpener.m_strFileName.GetBuffer(), 
					pcOpener.m_iPointCloudChoice-1);
				root->addChild(cs.get());
			} else {
				osg::ref_ptr<CloudStation> cs2 = tmtRW.Read(pcOpener.m_strFileName.GetBuffer(), 
					pcOpener.m_iTriangleChoice-1);
				root->addChild(cs2.get());
			}
		}
#endif

		AfxMessageBox("第二步，浏览、操纵点云/三角网");

		return root.get();
	}

	GUIEventHandlerArr PointCloudProcessor::CreateGUIEventHandlerArr()
	{
		GUIEventHandlerArr local_handlerArr;
		
		osg::ref_ptr<Controller> ctrl = new Controller;
		local_handlerArr.push_back(ctrl.get());
		this->handlerArr.push_back(ctrl);

		AfxMessageBox("创建控制器！");

#ifndef _DEBUG
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		setter = new CPropertySheet("点云控制器");
		
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

		setter->Create();
		setter->ShowWindow(SW_SHOW);
#endif

		return local_handlerArr;
	}
}