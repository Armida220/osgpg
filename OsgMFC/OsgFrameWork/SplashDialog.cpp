// SplashDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OsgFrameWork.h"
#include "SplashDialog.h"


// CSplashDialog �Ի���

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


// CSplashDialog ��Ϣ�������
