// PointCloudLODGenerator.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PointCloudLODGenerator.h"
#include "WaitDialog.h"
#include "LODParaSetter.h"

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
	*pobj = new FC::PointCloudLODGenerator;
	return *pobj != NULL;
}

namespace FC {
	osg::Node* PointCloudLODGenerator::CreateSceneData()
	{
		setlocale(LC_ALL, "chs");

		AfxMessageBox("��һ����ѡ�������LOD�ĵ����ļ�");

		CFileDialog dlg(TRUE, "ѡ�������LOD�ĵ����ļ�", NULL, 0, "All(*.*)|*.*|");
		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("û��ѡ���ļ���������������");
			return 0;
		}
		CString strFileName = dlg.GetPathName();

		AfxMessageBox("�ڶ���������Ϊosg\\ive��ʽ�����ļ�");
		CFileDialog saveDlg(FALSE, "ѡ�񱣴��ļ���", NULL, 0, "osg�ļ�(*.osg)|*.osg|ive�ļ�(*.ive)|*.ive|");
		if(saveDlg.DoModal()!=IDOK) {
			AfxMessageBox("û��ѡ�񱣴��ļ���, ������������");
			return 0;
		}
		CString strSaveFileName = saveDlg.GetPathName();

		/*
		AfxMessageBox("�����������ò���");
				LODParaSetter setter;
				float sampleRatio=0.1;
				unsigned int targetNumOnLeaf = 100;
				if(setter.DoModal()==IDOK) {
					sampleRatio = setter.m_sampleRatio;
					targetNumOnLeaf = setter.m_targetNumOnLeaf;
				}*/
		

		AfxMessageBox("����������ȡ�����ļ�������ת���������ĵȴ�...");

		setWorkingPath();

		CString cmd = "PointsDividor.exe";
		CString par;
		par.Format("\"%s\"  \"%s\"", strFileName, strSaveFileName);
		//AfxMessageBox(par);
		SHELLEXECUTEINFO shellinfo = runConsole(cmd, par);

		if(shellinfo.hProcess==NULL) {
			AfxMessageBox("�޷���Ӧ�ó���PointsDividor.exe��������������");
			return 0;
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

		osg::Node* ret = osgDB::readNodeFile(string(strSaveFileName));
		sceneRoot = ret;

		if(ret==0) {
			AfxMessageBox("�޷���ȡ���ļ���");
			return 0;
		}

		return ret;
	}

	void PointCloudLODGenerator::Release()
	{
		delete this;
	}
}