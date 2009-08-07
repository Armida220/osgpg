#pragma once
#include "afxcmn.h"

#include "resource.h"
// CWaitDialog 对话框

class CWaitDialog : public CDialog
{
	DECLARE_DYNAMIC(CWaitDialog)

public:
	CWaitDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progBar;
	virtual BOOL OnInitDialog();
};
