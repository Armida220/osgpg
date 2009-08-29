// OsgFrameWorkDoc.cpp : COsgFrameWorkDoc 类的实现
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


// COsgFrameWorkDoc 构造/析构

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

	CFileDialog dlg(TRUE, "打开osg、ive文件或工作流插件", NULL, 0, "osg文件(*.osg)|*.osg|ive文件(*.ive)|*.ive|plugin插件(*.plugin)|*.plugin|其他文件(*.*)|*.*||");
	if(dlg.DoModal()!=IDOK) {
		AfxMessageBox("没有选择任何文件！");
		if (!CDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}
	CString name = dlg.GetPathName();
	CString ext = dlg.GetFileExt();

	if( ext.CompareNoCase("plugin")==0 ) {
		if( m_pluginManager.SetWorkFlowPlugin(name) )
		{
			AfxMessageBox("成功加载插件！");
		}
	} else {
		m_strOsgFileName = name;
	}

	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}




// COsgFrameWorkDoc 序列化

void COsgFrameWorkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// COsgFrameWorkDoc 诊断

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


// COsgFrameWorkDoc 命令

void COsgFrameWorkDoc::OnFileOpen()
{
	//TODO add other control plugins
/*
	CFileDialog dlg(TRUE, "选择工作流插件", NULL, 0, "DLL文件(*.dll)|*.dll|");
	if(dlg.DoModal()!=IDOK) {
		AfxMessageBox("没有选择任何插件，接下来使用默认工作流！");
		if (!CDocument::OnOpenDocument(lpszPathName))
			return FALSE;
		return TRUE;
	}
	CString strDllFileName = dlg.GetPathName();

	CString strDllFileName(lpszPathName);

	if( m_pluginManager.AddWorkFlowPlugin(strDllFileName) )
	{
		AfxMessageBox("成功加载插件！");
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
			AfxMessageBox("成功加载插件！");
		}
	} else {
		m_strOsgFileName = name;
	}

	return TRUE;
}

void COsgFrameWorkDoc::OnFileSave()
{
	CFileDialog dlg(FALSE, "保存场景", NULL, 0, "osg文件(*.osg)|*.osg|ive文件(*.ive)|*.ive|");
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
			AfxMessageBox("无法保存到指定路径！");
		} else {
			AfxMessageBox("保存成功！");
		}
	}
}


void COsgFrameWorkDoc::OnFileAddnew()
{
	CFileDialog dlg(TRUE, "打开osg、ive文件", NULL, 0, "osg文件(*.osg)|*.osg|ive文件(*.ive)|*.ive|其他文件(*.*)|*.*||");
	if(dlg.DoModal()!=IDOK) {
		AfxMessageBox("没有选择任何文件！");
		return;
	}
	CString name = dlg.GetPathName();
	string openName(name);

	CMDIChildWnd* pChild=(CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
	COsgFrameWorkView* pView=(COsgFrameWorkView*)pChild->GetActiveView();
	if(!pView)
		return;

	setlocale(LC_ALL, "chs");

	//默认添加至第0号view
	if( pView->m_frame.AddModel(openName, 0) ) {
		AfxMessageBox("新文档成功附加至当前文档！");
	} else {
		AfxMessageBox("附加文档失败！");
	}

}

void COsgFrameWorkDoc::OnFileSaveAs()
{
	OnFileSave();
}
