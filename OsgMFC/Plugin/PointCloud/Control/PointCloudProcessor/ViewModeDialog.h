#pragma once

#include "resource.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
// ViewModeDialog 对话框

class ViewModeDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(ViewModeDialog)

public:
	ViewModeDialog();
	virtual ~ViewModeDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_VIEWMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	Controller* ctrl;
	afx_msg void OnBnClickedButtonView();
};
