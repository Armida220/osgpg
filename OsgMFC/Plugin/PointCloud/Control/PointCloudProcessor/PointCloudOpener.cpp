// PointCloudOpener.cpp : 实现文件
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "PointCloudOpener.h"


// PointCloudOpener 对话框

IMPLEMENT_DYNAMIC(PointCloudOpener, CDialog)

PointCloudOpener::PointCloudOpener(CWnd* pParent /*=NULL*/)
	: CDialog(PointCloudOpener::IDD, pParent)
	, m_strFileName(_T(""))
{
	m_iTriangleChoice = 0;
	m_iPointCloudChoice = 0;
}

PointCloudOpener::~PointCloudOpener()
{
}

void PointCloudOpener::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strFileName);
}


BEGIN_MESSAGE_MAP(PointCloudOpener, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &PointCloudOpener::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDOK, &PointCloudOpener::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_POINTCLOUD_NONE, &PointCloudOpener::OnBnClickedRadioPointcloudNone)
	ON_BN_CLICKED(IDC_RADIO_POINTCLOUD_RAW, &PointCloudOpener::OnBnClickedRadioPointcloudRaw)
	ON_BN_CLICKED(IDC_RADIO_POINTCLOUD_KD, &PointCloudOpener::OnBnClickedRadioPointcloudKd)
	ON_BN_CLICKED(IDC_RADIO_POINTCLOUD_OSG, &PointCloudOpener::OnBnClickedRadioPointcloudOsg)
	ON_BN_CLICKED(IDC_RADIO_TRIANGLE_NONE, &PointCloudOpener::OnBnClickedRadioTriangleNone)
	ON_BN_CLICKED(IDC_RADIO_TRIANGLE_RAW, &PointCloudOpener::OnBnClickedRadioTriangleRaw)
	ON_BN_CLICKED(IDC_RADIO_TRIANGLE_PLY, &PointCloudOpener::OnBnClickedRadioTrianglePly)
	ON_BN_CLICKED(IDC_RADIO_TRIANGLE_OSG, &PointCloudOpener::OnBnClickedRadioTriangleOsg)
END_MESSAGE_MAP()


// PointCloudOpener 消息处理程序

void PointCloudOpener::OnBnClickedButtonOpen()
{
	CFileDialog dlg(TRUE, "选择文件", NULL, 0, "All(*.*)|*.*|");
	if(dlg.DoModal()!=IDOK) {
		return;
	}
	this->m_strFileName = dlg.GetPathName();

	UpdateData(FALSE);
}

void PointCloudOpener::OnBnClickedOk()
{
	if( (m_iPointCloudChoice==0 &&	m_iTriangleChoice==0) ||
		(m_iPointCloudChoice!=0 &&	m_iTriangleChoice!=0) ) {
			AfxMessageBox("不能同时打开点云与三角网或是都不打开！");
			return;
	}
	OnOK();
}

void PointCloudOpener::OnBnClickedRadioPointcloudNone()
{
	m_iPointCloudChoice = 0;
}

void PointCloudOpener::OnBnClickedRadioPointcloudRaw()
{
	m_iPointCloudChoice = 1;
}

void PointCloudOpener::OnBnClickedRadioPointcloudKd()
{
	m_iPointCloudChoice = 2;
}

void PointCloudOpener::OnBnClickedRadioPointcloudOsg()
{
	m_iPointCloudChoice = 3;
}

void PointCloudOpener::OnBnClickedRadioTriangleNone()
{
	m_iTriangleChoice = 0;
}

void PointCloudOpener::OnBnClickedRadioTriangleRaw()
{
	m_iTriangleChoice = 1;
}

void PointCloudOpener::OnBnClickedRadioTrianglePly()
{
	m_iTriangleChoice = 3;
}

void PointCloudOpener::OnBnClickedRadioTriangleOsg()
{
	m_iTriangleChoice = 2;
}
