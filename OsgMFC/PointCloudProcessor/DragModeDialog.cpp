// DragModeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "DragModeDialog.h"


// DragModeDialog 对话框

IMPLEMENT_DYNAMIC(DragModeDialog, CPropertyPage)

DragModeDialog::DragModeDialog()
	: CPropertyPage(DragModeDialog::IDD)
{

}

DragModeDialog::~DragModeDialog()
{
}

void DragModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DragModeDialog, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DRAG, &DragModeDialog::OnBnClickedButtonDrag)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &DragModeDialog::OnBnClickedButtonUndo)
	ON_BN_CLICKED(IDC_BUTTON_REDO, &DragModeDialog::OnBnClickedButtonRedo)
END_MESSAGE_MAP()


// DragModeDialog 消息处理程序

void DragModeDialog::OnBnClickedButtonDrag()
{
	AfxMessageBox("进入拖拽模式！");
	ctrl->SetMode(Controller::DRAG);
	ctrl->SetDragMode(Controller::BEGIN);
}

void DragModeDialog::OnBnClickedButtonUndo()
{
	if(ctrl->GetMode()==Controller::DRAG) {
		ctrl->undo();
	}
}

void DragModeDialog::OnBnClickedButtonRedo()
{
	if(ctrl->GetMode()==Controller::DRAG) {
		ctrl->redo();
	}
}
