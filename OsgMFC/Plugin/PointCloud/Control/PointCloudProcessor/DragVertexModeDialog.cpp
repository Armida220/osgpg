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
    ON_BN_CLICKED(IDC_BUTTON1, &DragVertexModeDialog::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &DragVertexModeDialog::OnBnClickedButton2)
END_MESSAGE_MAP()


// DragVertexModeDialog ��Ϣ�������

void DragVertexModeDialog::OnBnClickedButtonDragvertex()
{
	//AfxMessageBox("������ק����ģʽ������ģʽֻ�������������ã�");
	ctrl->SetMode(Controller::DRAG_VERTEX);
	ctrl->SetDragVertexMode(Controller::BEGIN);
}

void DragVertexModeDialog::OnBnClickedButton1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    ctrl->SetMode(Controller::DRAGORPICK_EDGE);
}

void DragVertexModeDialog::OnBnClickedButton2()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    ctrl->SetMode(Controller::DRAG_TRIFACE);
}
