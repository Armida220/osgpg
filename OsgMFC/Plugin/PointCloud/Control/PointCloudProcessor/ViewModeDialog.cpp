// ViewModeDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "ViewModeDialog.h"


// ViewModeDialog �Ի���

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


// ViewModeDialog ��Ϣ�������

void ViewModeDialog::OnBnClickedButtonView()
{
	//AfxMessageBox("����۲�ģʽ��");
	ctrl->SetMode(Controller::VIEW);
	if(ctrl->GetDragMode()!=Controller::FINISH)
		ctrl->SetDragMode(Controller::FINISH);
	if(ctrl->GetDragVertexMode()!=Controller::FINISH)
		ctrl->SetDragVertexMode(Controller::FINISH);
}
