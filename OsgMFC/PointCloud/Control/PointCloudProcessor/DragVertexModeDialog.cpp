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
END_MESSAGE_MAP()


// DragVertexModeDialog 消息处理程序

void DragVertexModeDialog::OnBnClickedButtonDragvertex()
{
	AfxMessageBox("进入拖拽顶点模式！（该模式只对三角网起作用）");
	ctrl->SetMode(Controller::DRAG_VERTEX);
	ctrl->SetDragVertexMode(Controller::BEGIN);
}
