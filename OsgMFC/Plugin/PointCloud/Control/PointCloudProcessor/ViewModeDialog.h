#pragma once

#include "resource.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
// ViewModeDialog �Ի���

class ViewModeDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(ViewModeDialog)

public:
	ViewModeDialog();
	virtual ~ViewModeDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_VIEWMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	Controller* ctrl;
	afx_msg void OnBnClickedButtonView();
};
