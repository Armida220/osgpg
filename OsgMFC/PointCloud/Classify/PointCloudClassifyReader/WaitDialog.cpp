// WaitDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PointCloudClassifyReader.h"
#include "WaitDialog.h"


// CWaitDialog 对话框

IMPLEMENT_DYNAMIC(CWaitDialog, CDialog)

CWaitDialog::CWaitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDialog::IDD, pParent)
{
}

CWaitDialog::~CWaitDialog()
{
}

void CWaitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_READER, m_progBar);
}


BEGIN_MESSAGE_MAP(CWaitDialog, CDialog)
END_MESSAGE_MAP()


// CWaitDialog 消息处理程序

BOOL CWaitDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_progBar.SetRange(0,400);
	m_progBar.SetStep(2);
	m_progBar.SetPos(0);

	m_progBar.ShowWindow(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
