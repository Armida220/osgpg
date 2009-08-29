// OsgFrameWorkDoc.cpp : COsgFrameWorkDoc ���ʵ��
//

#include "stdafx.h"
#include "OsgFrameWork.h"

#include "OsgFrameWorkDoc.h"
#include "OsgFrameWorkView.h"
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COsgFrameWorkDoc

IMPLEMENT_DYNCREATE(COsgFrameWorkDoc, CDocument)

BEGIN_MESSAGE_MAP(COsgFrameWorkDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &COsgFrameWorkDoc::OnFileOpen)
	//ON_COMMAND(ID_FILE_NEW, &COsgFrameWorkDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &COsgFrameWorkDoc::OnFileSave)
	ON_COMMAND(ID_FILE_ADDNEW, &COsgFrameWorkDoc::OnFileAddnew)
	ON_COMMAND(ID_FILE_SAVE_AS, &COsgFrameWorkDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// COsgFrameWorkDoc ����/����

COsgFrameWorkDoc::COsgFrameWorkDoc()
{
	m_strOsgFileName = "";
}

COsgFrameWorkDoc::~COsgFrameWorkDoc()
{
}

BOOL COsgFrameWorkDoc::OnNewDocument()
{
	m_strOsgFileName = "";

	CFileDialog dlg(TRUE, "��osg��ive�ļ����������", NULL, 0, "osg�ļ�(*.osg)|*.osg|ive�ļ�(*.ive)|*.ive|plugin���(*.plugin)|*.plugin|�����ļ�(*.*)|*.*||");
	if(dlg.DoModal()!=IDOK) {
		AfxMessageBox("û��ѡ���κ��ļ���");
		if (!CDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}
	CString name = dlg.GetPathName();
	CString ext = dlg.GetFileExt();

	if( ext.CompareNoCase("plugin")==0 ) {
		if( m_pluginManager.SetWorkFlowPlugin(name) )
		{
			AfxMessageBox("�ɹ����ز����");
		}
	} else {
		m_strOsgFileName = name;
	}

	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}




// COsgFrameWorkDoc ���л�

void COsgFrameWorkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// COsgFrameWorkDoc ���

#ifdef _DEBUG
void COsgFrameWorkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COsgFrameWorkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COsgFrameWorkDoc ����

void COsgFrameWorkDoc::OnFileOpen()
{
	//TODO add other control plugins
/*
	CFileDialog dlg(TRUE, "ѡ���������", NULL, 0, "DLL�ļ�(*.dll)|*.dll|");
	if(dlg.DoModal()!=IDOK) {
		AfxMessageBox("û��ѡ���κβ����������ʹ��Ĭ�Ϲ�������");
		if (!CDocument::OnOpenDocument(lpszPathName))
			return FALSE;
		return TRUE;
	}
	CString strDllFileName = dlg.GetPathName();

	CString strDllFileName(lpszPathName);

	if( m_pluginManager.AddWorkFlowPlugin(strDllFileName) )
	{
		AfxMessageBox("�ɹ����ز����");
	}
*/
}

BOOL COsgFrameWorkDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CString name(lpszPathName);
	string ext_ = osgDB::getFileExtension(string(name));
	CString ext( ext_.c_str() );

	if( ext.CompareNoCase("plugin")==0 ) {
		if( m_pluginManager.SetWorkFlowPlugin(name) )
		{
			AfxMessageBox("�ɹ����ز����");
		}
	} else {
		m_strOsgFileName = name;
	}

	return TRUE;
}

void COsgFrameWorkDoc::OnFileSave()
{
	CFileDialog dlg(FALSE, "���泡��", NULL, 0, "osg�ļ�(*.osg)|*.osg|ive�ļ�(*.ive)|*.ive|");
	if(dlg.DoModal()!=IDOK) {
		return;
	}
	CString name = dlg.GetPathName();
	string saveName(name);

	CMDIChildWnd* pChild=(CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
	COsgFrameWorkView* pView=(COsgFrameWorkView*)pChild->GetActiveView();
	if(!pView)
		return;

	setlocale(LC_ALL, "chs");

	//here we use view 0 as the default scene view which contains all the sceneData
	osg::Node* node = pView->m_frame.GetViewer()->getView(0)->getSceneData();
	if(node) {
		if( !osgDB::writeNodeFile(*node, saveName) ) {
			AfxMessageBox("�޷����浽ָ��·����");
		} else {
			AfxMessageBox("����ɹ���");
		}
	}
}


void COsgFrameWorkDoc::OnFileAddnew()
{
	CFileDialog dlg(TRUE, "��osg��ive�ļ�", NULL, 0, "osg�ļ�(*.osg)|*.osg|ive�ļ�(*.ive)|*.ive|�����ļ�(*.*)|*.*||");
	if(dlg.DoModal()!=IDOK) {
		AfxMessageBox("û��ѡ���κ��ļ���");
		return;
	}
	CString name = dlg.GetPathName();
	string openName(name);

	CMDIChildWnd* pChild=(CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
	COsgFrameWorkView* pView=(COsgFrameWorkView*)pChild->GetActiveView();
	if(!pView)
		return;

	setlocale(LC_ALL, "chs");

	//Ĭ���������0��view
	if( pView->m_frame.AddModel(openName, 0) ) {
		AfxMessageBox("���ĵ��ɹ���������ǰ�ĵ���");
	} else {
		AfxMessageBox("�����ĵ�ʧ�ܣ�");
	}

}

void COsgFrameWorkDoc::OnFileSaveAs()
{
	OnFileSave();
}
