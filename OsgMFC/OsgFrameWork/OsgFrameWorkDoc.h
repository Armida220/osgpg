// OsgFrameWorkDoc.h : COsgFrameWorkDoc ��Ľӿ�
//


#pragma once
#include "PluginManager.h"

class COsgFrameWorkDoc : public CDocument
{
protected: // �������л�����
	COsgFrameWorkDoc();
	DECLARE_DYNCREATE(COsgFrameWorkDoc)

// ����
public:
	CString m_strOsgFileName;
	FC::PluginManager m_pluginManager;

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~COsgFrameWorkDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileSave();
	afx_msg void OnFileAddnew();
	afx_msg void OnFileSaveAs();
};


