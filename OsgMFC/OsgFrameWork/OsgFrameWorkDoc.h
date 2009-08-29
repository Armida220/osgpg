// OsgFrameWorkDoc.h : COsgFrameWorkDoc 类的接口
//


#pragma once
#include "PluginManager.h"

class COsgFrameWorkDoc : public CDocument
{
protected: // 仅从序列化创建
	COsgFrameWorkDoc();
	DECLARE_DYNCREATE(COsgFrameWorkDoc)

// 属性
public:
	CString m_strOsgFileName;
	FC::PluginManager m_pluginManager;

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~COsgFrameWorkDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileSave();
	afx_msg void OnFileAddnew();
	afx_msg void OnFileSaveAs();
};


