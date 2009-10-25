#pragma once

#include "ImgMeasurementDlg.h"
// ImageOpener 对话框

class ImageOpener : public CPropertyPage
{
	DECLARE_DYNAMIC(ImageOpener)

public:
	ImageOpener();   // 标准构造函数
	virtual ~ImageOpener();

// 对话框数据
	enum { IDD = IDD_DIALOG_IMAGEOPENER };

	CImgMeasurementDlg* imgMeasurer;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonOut();
};
