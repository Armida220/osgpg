// OsgFrameWorkView.h : COsgFrameWorkView ��Ľӿ�
//


#pragma once

#include "osgViewerFramework.h"

class COsgFrameWorkView : public CView
{
protected: // �������л�����
	COsgFrameWorkView();
	DECLARE_DYNCREATE(COsgFrameWorkView)

// ����
public:
	COsgFrameWorkDoc* GetDocument() const;

	FC::ViewerFramework m_frame;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~COsgFrameWorkView();
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnAddControllerPlugin();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // OsgFrameWorkView.cpp �еĵ��԰汾
inline COsgFrameWorkDoc* COsgFrameWorkView::GetDocument() const
   { return reinterpret_cast<COsgFrameWorkDoc*>(m_pDocument); }
#endif

