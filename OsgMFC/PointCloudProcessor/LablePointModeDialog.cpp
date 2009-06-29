// LablePointModeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "LablePointModeDialog.h"


// LablePointModeDialog 对话框

IMPLEMENT_DYNAMIC(LablePointModeDialog, CPropertyPage)

LablePointModeDialog::LablePointModeDialog()
	: CPropertyPage(LablePointModeDialog::IDD)
	, m_iLableID(0)
{

}

LablePointModeDialog::~LablePointModeDialog()
{
}

void LablePointModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LABLE_ID, m_iLableID);
}


BEGIN_MESSAGE_MAP(LablePointModeDialog, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_LABLE_POINT, &LablePointModeDialog::OnBnClickedButtonLablePoint)
	ON_BN_CLICKED(IDC_BUTTON_LABLE_ADD, &LablePointModeDialog::OnBnClickedButtonLableAdd)
	ON_BN_CLICKED(IDC_BUTTON_LABLE_MINUS, &LablePointModeDialog::OnBnClickedButtonLableMinus)
END_MESSAGE_MAP()


// LablePointModeDialog 消息处理程序

void LablePointModeDialog::OnBnClickedButtonLablePoint()
{
	AfxMessageBox("进入标记点模式！");
	ctrl->SetMode(Controller::LABEL_POINT);
}

void LablePointModeDialog::OnBnClickedButtonLableAdd()
{
	if(ctrl->GetMode()==Controller::LABEL_POINT) {
		ctrl->SetCurID(ctrl->GetCurID()+1);
		this->m_iLableID = ctrl->GetCurID();
		UpdateData(FALSE);
	}
}

void LablePointModeDialog::OnBnClickedButtonLableMinus()
{
	if(ctrl->GetMode()==Controller::LABEL_POINT) {
		ctrl->SetCurID(ctrl->GetCurID()-1);
		this->m_iLableID = ctrl->GetCurID();
		UpdateData(FALSE);
	}
}
