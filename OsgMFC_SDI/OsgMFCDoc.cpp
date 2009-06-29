// OsgMFCDoc.cpp : COsgMFCDoc 类的实现
//

#include "stdafx.h"
#include "OsgMFC.h"

#include "OsgMFCDoc.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COsgMFCDoc

IMPLEMENT_DYNCREATE(COsgMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(COsgMFCDoc, CDocument)
END_MESSAGE_MAP()


// COsgMFCDoc 构造/析构

COsgMFCDoc::COsgMFCDoc()
{
}

COsgMFCDoc::~COsgMFCDoc()
{
}

BOOL COsgMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// COsgMFCDoc 序列化

void COsgMFCDoc::Serialize(CArchive& ar)
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


// COsgMFCDoc 诊断

#ifdef _DEBUG
void COsgMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COsgMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COsgMFCDoc 命令

BOOL COsgMFCDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}
