// PickTriangleModeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "PickTriangleModeDialog.h"

// PickTriangleModeDialog 对话框

IMPLEMENT_DYNAMIC(PickTriangleModeDialog, CPropertyPage)

PickTriangleModeDialog::PickTriangleModeDialog()
	: CPropertyPage(PickTriangleModeDialog::IDD)
	, m_ax(0)
	, m_ay(0)
	, m_az(0)
	, m_bx(0)
	, m_by(0)
	, m_bz(0)
	, m_cx(0)
	, m_cy(0)
	, m_cz(0)
{

}

PickTriangleModeDialog::~PickTriangleModeDialog()
{
}

void PickTriangleModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AX, m_ax);
	DDX_Text(pDX, IDC_EDIT_AY, m_ay);
	DDX_Text(pDX, IDC_EDIT_AZ, m_az);
	DDX_Text(pDX, IDC_EDIT_BX, m_bx);
	DDX_Text(pDX, IDC_EDIT_BY, m_by);
	DDX_Text(pDX, IDC_EDIT_BZ, m_bz);
	DDX_Text(pDX, IDC_EDIT_CX, m_cx);
	DDX_Text(pDX, IDC_EDIT_CY, m_cy);
	DDX_Text(pDX, IDC_EDIT_CZ, m_cz);
}


BEGIN_MESSAGE_MAP(PickTriangleModeDialog, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PICK_TRIANGLE, &PickTriangleModeDialog::OnBnClickedButtonPickTriangle)
	ON_BN_CLICKED(IDC_BUTTON_CUR_TRIANGLE, &PickTriangleModeDialog::OnBnClickedButtonCurTriangle)
END_MESSAGE_MAP()


// PickTriangleModeDialog 消息处理程序

void PickTriangleModeDialog::OnBnClickedButtonPickTriangle()
{
	AfxMessageBox("进入选择三角网模式！");
	ctrl->SetMode(Controller::PICK_TRIANGLE);
}

void PickTriangleModeDialog::OnBnClickedButtonCurTriangle()
{
	const Triangle& tri
		= ctrl->GetTriangle();

	m_ax = tri.vertexs[0].x();
	m_ay = tri.vertexs[0].y();
	m_az = tri.vertexs[0].z();

	m_bx = tri.vertexs[1].x();
	m_by = tri.vertexs[1].y();
	m_bz = tri.vertexs[1].z();

	m_cx = tri.vertexs[2].x();
	m_cy = tri.vertexs[2].y();
	m_cz = tri.vertexs[2].z();

	UpdateData(FALSE);
}
