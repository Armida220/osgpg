// ImageOpener.cpp : 实现文件
//

#include "stdafx.h"
#include "PointCloudProcessor.h"
#include "ImageOpener.h"
#include "ImgMeasurementDlg.h"

#include <iostream>
#include <fstream>
using namespace std;

// ImageOpener 对话框

IMPLEMENT_DYNAMIC(ImageOpener, CPropertyPage)

ImageOpener::ImageOpener()
	: CPropertyPage(ImageOpener::IDD)
{
	imgMeasurer = 0;
}

ImageOpener::~ImageOpener()
{
	if(imgMeasurer) {
		delete imgMeasurer;
		imgMeasurer = 0;
	}
}

void ImageOpener::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ImageOpener, CPropertyPage)
	ON_BN_CLICKED(IDOK, &ImageOpener::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ImageOpener::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_OUT, &ImageOpener::OnBnClickedButtonOut)
END_MESSAGE_MAP()


// ImageOpener 消息处理程序

void ImageOpener::OnBnClickedOk()
{
	if(imgMeasurer==0) {
		imgMeasurer = new CImgMeasurementDlg;
		imgMeasurer->Create(IDD_IMGMEASUREMENT_DIALOG);
	}

	imgMeasurer->ShowWindow(SW_SHOW);

	//OnOK();
}

void ImageOpener::OnBnClickedCancel()
{
	if(imgMeasurer) {
		delete imgMeasurer;
		imgMeasurer = 0;
	}

	OnBnClickedOk();
	//OnCancel();
}

void ImageOpener::OnBnClickedButtonOut()
{
	setlocale(LC_ALL, "chs");
	if(imgMeasurer==0) {
		AfxMessageBox("目前没有选择任何点！");
		return;
	}

	CFileDialog fileDlg(FALSE, "选择输出文件路径");

	CString name;
	if(IDOK==fileDlg.DoModal())
	{
		name = fileDlg.GetPathName();

		ImagePointArr ipa;
		imgMeasurer->GetImagePointArray(ipa);

		std::ofstream out(name);
		out<<ipa.size()<<endl;
		for(int i=0; i<ipa.size(); ++i)
		{
			const ImagePoint<long>& ip = ipa[i];
			out<<ip.x<<" "<<ip.y<<endl;
		}
		out.close();

		AfxMessageBox("图像点保存成功！");
	}
}
