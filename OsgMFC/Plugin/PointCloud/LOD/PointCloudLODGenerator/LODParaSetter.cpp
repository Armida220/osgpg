// LODParaSetter.cpp : 实现文件
//

#include "stdafx.h"
#include "PointCloudLODGenerator.h"
#include "LODParaSetter.h"


// LODParaSetter 对话框

IMPLEMENT_DYNAMIC(LODParaSetter, CDialog)

LODParaSetter::LODParaSetter(CWnd* pParent /*=NULL*/)
	: CDialog(LODParaSetter::IDD, pParent)
	, m_sampleRatio(0)
	, m_targetNumOnLeaf(0)
{

}

LODParaSetter::~LODParaSetter()
{
}

void LODParaSetter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SAMPLE, m_sampleRatio);
	DDX_Text(pDX, IDC_EDIT_TARGET, m_targetNumOnLeaf);
}


BEGIN_MESSAGE_MAP(LODParaSetter, CDialog)
	ON_BN_CLICKED(IDOK, &LODParaSetter::OnBnClickedOk)
END_MESSAGE_MAP()


// LODParaSetter 消息处理程序

void LODParaSetter::OnBnClickedOk()
{
	UpdateData();
	OnOK();
}
