// ViewModeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "ViewModeDialog.h"


// ViewModeDialog 对话框

IMPLEMENT_DYNAMIC(ViewModeDialog, CPropertyPage)

ViewModeDialog::ViewModeDialog()
	: CPropertyPage(ViewModeDialog::IDD)
{

}

ViewModeDialog::~ViewModeDialog()
{
}

void ViewModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ViewModeDialog, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, &ViewModeDialog::OnBnClickedButtonView)
END_MESSAGE_MAP()


// ViewModeDialog 消息处理程序

void ViewModeDialog::OnBnClickedButtonView()
{
	//AfxMessageBox("进入观察模式！");
	ctrl->SetMode(Controller::VIEW);
	if(ctrl->GetDragMode()!=Controller::FINISH)
		ctrl->SetDragMode(Controller::FINISH);
	if(ctrl->GetDragVertexMode()!=Controller::FINISH)
		ctrl->SetDragVertexMode(Controller::FINISH);
}
