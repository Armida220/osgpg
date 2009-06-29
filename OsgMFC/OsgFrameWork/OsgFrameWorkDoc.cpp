// OsgFrameWorkDoc.cpp : COsgFrameWorkDoc ���ʵ��
//

#include "stdafx.h"
#include "OsgFrameWork.h"

#include "OsgFrameWorkDoc.h"
#include "OsgFrameWorkView.h"
#include <osgDB/WriteFile>
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

	CFileDialog dlg(TRUE, "��osg��ive�ļ����������", NULL, 0, "osg�ļ�(*.osg)|*.osg|ive�ļ�(*.ive)|*.ive|plugin���(*.plugin)|*.plugin|");
	if(dlg.DoModal()!=IDOK) {
		AfxMessageBox("û��ѡ���κ��ļ���");
		if (!CDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}
	CString name = dlg.GetPathName();
	CString ext = dlg.GetFileExt();

	if( ext.CompareNoCase("osg")==0 || 
		ext.CompareNoCase("ive")==0 )
		m_strOsgFileName = name;
	else
		if( m_pluginManager.AddPlugin(name) )
		{
			AfxMessageBox("�ɹ����ز����");
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

	if( m_pluginManager.AddPlugin(strDllFileName) )
	{
		AfxMessageBox("�ɹ����ز����");
	}
*/
}

BOOL COsgFrameWorkDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;
/*
	CString strDllFileName(lpszPathName);

	if( m_pluginManager.AddPlugin(strDllFileName) )
	{
		AfxMessageBox("�ɹ����ز����");
	}
*/
	CString name(lpszPathName);
	string ext_ = osgDB::getFileExtension(string(name));
	CString ext( ext_.c_str() );

	if( ext.CompareNoCase("osg")==0 || 
		ext.CompareNoCase("ive")==0 )
		m_strOsgFileName = name;
	else
		if( m_pluginManager.AddPlugin(name) )
		{
			AfxMessageBox("�ɹ����ز����");
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

	osg::Node* node = pView->m_frame.GetViewer()->getSceneData();
	if(node) {
		if( !osgDB::writeNodeFile(*node, saveName) ) {
			AfxMessageBox("�޷����浽ָ��·����");
		} else {
			AfxMessageBox("����ɹ���");
		}
	}
}

