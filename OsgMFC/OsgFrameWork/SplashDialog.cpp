// SplashDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OsgFrameWork.h"
#include "SplashDialog.h"


// CSplashDialog 对话框

IMPLEMENT_DYNAMIC(CSplashDialog, CDialog)

CSplashDialog::CSplashDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDialog::IDD, pParent)
{

}

CSplashDialog::~CSplashDialog()
{
}

void CSplashDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplashDialog, CDialog)
END_MESSAGE_MAP()


// CSplashDialog 消息处理程序
