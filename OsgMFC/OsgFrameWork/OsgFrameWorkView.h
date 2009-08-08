// OsgFrameWorkView.h : COsgFrameWorkView 类的接口
//


#pragma once

#include "osgViewerFramework.h"

class COsgFrameWorkView : public CView
{
protected: // 仅从序列化创建
	COsgFrameWorkView();
	DECLARE_DYNCREATE(COsgFrameWorkView)

// 属性
public:
	COsgFrameWorkDoc* GetDocument() const;

	FC::ViewerFramework m_frame;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~COsgFrameWorkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnAddControllerPlugin();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // OsgFrameWorkView.cpp 中的调试版本
inline COsgFrameWorkDoc* COsgFrameWorkView::GetDocument() const
   { return reinterpret_cast<COsgFrameWorkDoc*>(m_pDocument); }
#endif

