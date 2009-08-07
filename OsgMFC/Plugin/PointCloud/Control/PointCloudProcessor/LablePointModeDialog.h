#pragma once

#include "resource.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
// LablePointModeDialog �Ի���

class LablePointModeDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(LablePointModeDialog)

public:
	LablePointModeDialog();
	virtual ~LablePointModeDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_LABLE_POINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	Controller* ctrl;
	afx_msg void OnBnClickedButtonLablePoint();
	afx_msg void OnBnClickedButtonLableAdd();
	afx_msg void OnBnClickedButtonLableMinus();
	UINT m_iLableID;
};
