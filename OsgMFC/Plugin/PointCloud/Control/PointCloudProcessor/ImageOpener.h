#pragma once

#include "ImgMeasurementDlg.h"
// ImageOpener �Ի���

class ImageOpener : public CPropertyPage
{
	DECLARE_DYNAMIC(ImageOpener)

public:
	ImageOpener();   // ��׼���캯��
	virtual ~ImageOpener();

// �Ի�������
	enum { IDD = IDD_DIALOG_IMAGEOPENER };

	CImgMeasurementDlg* imgMeasurer;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonOut();
};
