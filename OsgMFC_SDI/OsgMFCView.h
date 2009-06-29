// OsgMFCView.h : COsgMFCView 类的接口
//


#pragma once

#include "osgViewerFramework.h"
#include "PluginManager.h"

class COsgMFCView : public CView
{
protected: // 仅从序列化创建
	COsgMFCView();
	DECLARE_DYNCREATE(COsgMFCView)

// 属性
public:
	COsgMFCDoc* GetDocument() const;

	FC::ViewerFramework m_frame;
	bool m_Closed_yet;

	FC::PluginManager m_pluginManager;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~COsgMFCView();
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
	afx_msg void OnDestroy();
	afx_msg void OnOsgRun();
	afx_msg void OnLoadWorkflowPlugin();
	afx_msg void OnOsgStop();
};

#ifndef _DEBUG  // OsgMFCView.cpp 中的调试版本
inline COsgMFCDoc* COsgMFCView::GetDocument() const
   { return reinterpret_cast<COsgMFCDoc*>(m_pDocument); }
#endif

