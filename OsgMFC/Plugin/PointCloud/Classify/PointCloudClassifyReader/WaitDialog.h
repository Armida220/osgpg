#pragma once
#include "afxcmn.h"

#include "resource.h"
// CWaitDialog �Ի���

class CWaitDialog : public CDialog
{
	DECLARE_DYNAMIC(CWaitDialog)

public:
	CWaitDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaitDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progBar;
	virtual BOOL OnInitDialog();
};
