// ImgMountainParaSetter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageMountain.h"
#include "ImgMountainParaSetter.h"


// ImgMountainParaSetter �Ի���

IMPLEMENT_DYNAMIC(ImgMountainParaSetter, CDialog)

ImgMountainParaSetter::ImgMountainParaSetter(CWnd* pParent /*=NULL*/)
	: CDialog(ImgMountainParaSetter::IDD, pParent)
	, m_heightScale(20.0)
	, m_planescale(10.0)
{

}

ImgMountainParaSetter::~ImgMountainParaSetter()
{
}

void ImgMountainParaSetter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HEIGHTSCALE, m_heightScale);
	DDX_Text(pDX, IDC_EDIT_PLANESCALE, m_planescale);
}


BEGIN_MESSAGE_MAP(ImgMountainParaSetter, CDialog)
	ON_BN_CLICKED(IDOK, &ImgMountainParaSetter::OnBnClickedOk)
END_MESSAGE_MAP()


// ImgMountainParaSetter ��Ϣ�������

void ImgMountainParaSetter::OnBnClickedOk()
{
	UpdateData();
	OnOK();
}
