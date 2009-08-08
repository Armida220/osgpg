// BuildingSimplifier.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "BuildingSimplifier.h"
#include "WaitDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

#include <osg/Geode>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <direct.h>//for chdir


SHELLEXECUTEINFO runConsole(CString cmd,	CString par)
{
	SHELLEXECUTEINFO  ShExecInfo  =  {0};
	ShExecInfo.cbSize  =  sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask  =  SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd  =  NULL;
	ShExecInfo.lpVerb  =  NULL;
	ShExecInfo.lpFile  =  cmd;
	ShExecInfo.lpParameters  =  par;
	ShExecInfo.lpDirectory  =  NULL;
	ShExecInfo.nShow  =  SW_HIDE;
	ShExecInfo.hInstApp  =  NULL;
	ShellExecuteEx(&ShExecInfo);

	return ShExecInfo;
}

void setWorkingPath()
{
	//设置工作路径
	char path[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH, path);
	AfxTrace(path);
	GetModuleFileName(NULL, path, MAX_PATH);
	AfxTrace(path);
	string tmp(path);
	int postmp = tmp.find_last_of('\\');
	tmp[postmp]='\0';
	chdir(tmp.c_str());
}


//////////////////////////////////////////////////////////////////////////
bool CreateWorkFlowPlugin(void **pobj)
{
	*pobj = new FC::BuildingSimplifier;
	return *pobj != NULL;
}

namespace FC {
	void BuildingSimplifier::SetSceneData(osgViewer::CompositeViewer* viewer)
	{
		setlocale(LC_ALL, "chs");

		AfxMessageBox("第一步：简化建筑场景");

		setWorkingPath();

		//init
		osg::Group* root = viewer->getView(0)->getSceneData()->asGroup();

		CString cmd = "ModelSimplify.exe";
		CString par;
		SHELLEXECUTEINFO shellinfo = runConsole(cmd, par);

		if(shellinfo.hProcess==NULL) {
			AfxMessageBox("无法打开应用程序：ModelSimplify.exe！工作流结束！");
			return;
		}

#ifndef _DEBUG
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CWaitDialog* pWaitDlg = new CWaitDialog;
		pWaitDlg->Create(IDD_DIALOG_WAIT, NULL);
		pWaitDlg->ShowWindow(SW_SHOW);
#endif

		while (true) {
			DWORD signal = WaitForSingleObject(shellinfo.hProcess, 10);
			if ( signal == WAIT_OBJECT_0 || shellinfo.hProcess==NULL )	{
				break;
			}

			MSG msg;
			GetMessage(&msg, NULL, 0, 0);
			if (&msg!=NULL)	{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
#ifndef _DEBUG
			pWaitDlg->m_progBar.StepIt();
#endif
		}

#ifndef _DEBUG
		pWaitDlg->DestroyWindow();
		delete pWaitDlg;
#endif

		AfxMessageBox("第二步：浏览简化后建筑场景");
		CFileDialog dlg(TRUE, "选择简化后建筑场景文件", NULL, 0, "osg(*.osg)|*.osg|ive(*.ive)|*.ive|");
		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择文件，工作流结束！");
			return;
		}
		CString strFileName = dlg.GetPathName();

		osg::Node* ret = osgDB::readNodeFile(string(strFileName));
		sceneRoot = ret;

		if(ret==0) {
			AfxMessageBox("无法读取该文件！");
			return;
		}

		root->addChild(ret);
	}

	void BuildingSimplifier::Release()
	{
		delete this;
	}
}