// DragVertexModeDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "DragVertexModeDialog.h"


// DragVertexModeDialog �Ի���

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


// DragVertexModeDialog ��Ϣ�������

void DragVertexModeDialog::OnBnClickedButtonDragvertex()
{
	AfxMessageBox("������ק����ģʽ������ģʽֻ�������������ã�");
	ctrl->SetMode(Controller::DRAG_VERTEX);
	ctrl->SetDragVertexMode(Controller::BEGIN);
}
