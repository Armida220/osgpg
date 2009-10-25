// DragVertexModeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "DragVertexModeDialog.h"


// DragVertexModeDialog 对话框

IMPLEMENT_DYNAMIC(DragVertexModeDialog, CPropertyPage)

DragVertexModeDialog::DragVertexModeDialog()
	: CPropertyPage(DragVertexModeDialog::IDD)
{

}

DragVertexModeDialog::~DragVertexModeDialog()
{
}

void DragVertexModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DragVertexModeDialog, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DRAGVERTEX, &DragVertexModeDialog::OnBnClickedButtonDragvertex)
    ON_BN_CLICKED(IDC_BUTTON1, &DragVertexModeDialog::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &DragVertexModeDialog::OnBnClickedButton2)
END_MESSAGE_MAP()


// DragVertexModeDialog 消息处理程序

void DragVertexModeDialog::OnBnClickedButtonDragvertex()
{
	//AfxMessageBox("进入拖拽顶点模式！（该模式只对三角网起作用）");
	ctrl->SetMode(Controller::DRAG_VERTEX);
	ctrl->SetDragVertexMode(Controller::BEGIN);
}

void DragVertexModeDialog::OnBnClickedButton1()
{
    // TODO: 在此添加控件通知处理程序代码
    ctrl->SetMode(Controller::DRAGORPICK_EDGE);
}

void DragVertexModeDialog::OnBnClickedButton2()
{
    // TODO: 在此添加控件通知处理程序代码
    ctrl->SetMode(Controller::DRAG_TRIFACE);
}
