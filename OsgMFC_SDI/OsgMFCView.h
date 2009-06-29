// OsgMFCView.h : COsgMFCView ��Ľӿ�
//


#pragma once

#include "osgViewerFramework.h"
#include "PluginManager.h"

class COsgMFCView : public CView
{
protected: // �������л�����
	COsgMFCView();
	DECLARE_DYNCREATE(COsgMFCView)

// ����
public:
	COsgMFCDoc* GetDocument() const;

	FC::ViewerFramework m_frame;
	bool m_Closed_yet;

	FC::PluginManager m_pluginManager;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~COsgMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnOsgRun();
	afx_msg void OnLoadWorkflowPlugin();
	afx_msg void OnOsgStop();
};

#ifndef _DEBUG  // OsgMFCView.cpp �еĵ��԰汾
inline COsgMFCDoc* COsgMFCView::GetDocument() const
   { return reinterpret_cast<COsgMFCDoc*>(m_pDocument); }
#endif

