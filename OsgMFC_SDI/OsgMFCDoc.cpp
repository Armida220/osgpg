// OsgMFCDoc.cpp : COsgMFCDoc ���ʵ��
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


// COsgMFCDoc ����/����

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

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// COsgMFCDoc ���л�

void COsgMFCDoc::Serialize(CArchive& ar)
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


// COsgMFCDoc ���

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


// COsgMFCDoc ����

BOOL COsgMFCDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}
