// BuildingSimplifier.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "BuildingSimplifier.h"
#include "WaitDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

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
	//���ù���·��
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

		AfxMessageBox("��һ�����򻯽�������");

		setWorkingPath();

		//init
		osg::Group* root = viewer->getView(0)->getSceneData()->asGroup();

		CString cmd = "ModelSimplify.exe";
		CString par;
		SHELLEXECUTEINFO shellinfo = runConsole(cmd, par);

		if(shellinfo.hProcess==NULL) {
			AfxMessageBox("�޷���Ӧ�ó���ModelSimplify.exe��������������");
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

		AfxMessageBox("�ڶ���������򻯺�������");
		CFileDialog dlg(TRUE, "ѡ��򻯺��������ļ�", NULL, 0, "osg(*.osg)|*.osg|ive(*.ive)|*.ive|");
		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("û��ѡ���ļ���������������");
			return;
		}
		CString strFileName = dlg.GetPathName();

		osg::Node* ret = osgDB::readNodeFile(string(strFileName));
		sceneRoot = ret;

		if(ret==0) {
			AfxMessageBox("�޷���ȡ���ļ���");
			return;
		}

		root->addChild(ret);
	}

	void BuildingSimplifier::Release()
	{
		delete this;
	}
}