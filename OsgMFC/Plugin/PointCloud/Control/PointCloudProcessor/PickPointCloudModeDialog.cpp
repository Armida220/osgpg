// PickPointCloudModeDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "PickPointCloudModeDialog.h"


// PickPointCloudModeDialog �Ի���

IMPLEMENT_DYNAMIC(PickPointCloudModeDialog, CPropertyPage)

PickPointCloudModeDialog::PickPointCloudModeDialog()
	: CPropertyPage(PickPointCloudModeDialog::IDD)
	, m_triangleArea(0)
{

}

PickPointCloudModeDialog::~PickPointCloudModeDialog()
{
}

void PickPointCloudModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TRIANGLE_AREA, m_triangleArea);
}


BEGIN_MESSAGE_MAP(PickPointCloudModeDialog, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PICK_POINTCLOUD, &PickPointCloudModeDialog::OnBnClickedButtonPickPointcloud)
	ON_BN_CLICKED(IDC_BUTTON_TRIAREA, &PickPointCloudModeDialog::OnBnClickedButtonTriarea)
END_MESSAGE_MAP()


// PickPointCloudModeDialog ��Ϣ�������

void PickPointCloudModeDialog::OnBnClickedButtonPickPointcloud()
{
	AfxMessageBox("����ѡ�����ģʽ��");
	ctrl->SetMode(Controller::PICK_POINT_CLOUD);
}

void PickPointCloudModeDialog::OnBnClickedButtonTriarea()
{
	double area = ctrl->GetTriangleArea();
	if(area<0) {
		AfxMessageBox("����δѡ���κ���������");
	}
	m_triangleArea = area;
	UpdateData(FALSE);
}
