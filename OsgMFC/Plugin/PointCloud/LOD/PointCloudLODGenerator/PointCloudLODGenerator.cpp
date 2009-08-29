// PointCloudLODGenerator.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "PointCloudLODGenerator.h"
#include "WaitDialog.h"
#include "LODParaSetter.h"

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
	*pobj = new FC::PointCloudLODGenerator;
	return *pobj != NULL;
}

namespace FC {
	void PointCloudLODGenerator::SetSceneData(osgViewer::CompositeViewer* viewer)
	{
		setlocale(LC_ALL, "chs");

		AfxMessageBox("第一步：选择待生成LOD的点云文件");

		CFileDialog dlg(TRUE, "选择待生成LOD的点云文件", NULL, 0, "All(*.*)|*.*|");
		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择文件，工作流结束！");
			return;
		}
		CString strFileName = dlg.GetPathName();

		AfxMessageBox("第二步：保存为osg\\ive格式场景文件");
		CFileDialog saveDlg(FALSE, "选择保存文件名", NULL, 0, "osg文件(*.osg)|*.osg|ive文件(*.ive)|*.ive|");
		if(saveDlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择保存文件名, 工作流结束！");
			return;
		}
		CString strSaveFileName = saveDlg.GetPathName();

#ifndef _DEBUG
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
		AfxMessageBox("第三步，设置参数");
		LODParaSetter setter;
		float sampleRatio=0.001;
		unsigned int targetNumOnLeaf = 10000;
		double maxVisibleRange=800;
		double minVisibleRange=100;
		if(setter.DoModal()==IDOK) {
			sampleRatio = setter.m_sampleRatio;
			targetNumOnLeaf = setter.m_targetNumOnLeaf;
			maxVisibleRange = setter.maxBoundVisibleRange;
			minVisibleRange = setter.minBoundVisibleRange;
		}

		AfxMessageBox("第三步：读取点云文件并进行转换，请耐心等待...");

		setWorkingPath();

		CString cmd = "PointsDividor.exe";
		CString par;
		par.Format("\"%s\" \"%s\" %d %f %d %lf", strFileName, strSaveFileName, 
			16, sampleRatio, targetNumOnLeaf, maxVisibleRange, minVisibleRange);
		//AfxMessageBox(par);
		SHELLEXECUTEINFO shellinfo = runConsole(cmd, par);

		if(shellinfo.hProcess==NULL) {
			AfxMessageBox("无法打开应用程序：PointsDividor.exe！工作流结束！");
			return;
		}

#ifndef _DEBUG
		//AFX_MANAGE_STATE(AfxGetStaticModuleState());
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

		osg::Node* ret = osgDB::readNodeFile(string(strSaveFileName));
		sceneRoot = ret;

		if(ret==0) {
			AfxMessageBox("无法读取该文件！");
			return;
		}

		osg::Group* root = viewer->getView(0)->getSceneData()->asGroup();
		root->addChild(ret);
	}

	void PointCloudLODGenerator::Release()
	{
		delete this;
	}
}