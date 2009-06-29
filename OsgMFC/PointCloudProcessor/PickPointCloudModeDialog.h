#pragma once

#include "resource.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
// PickPointCloudModeDialog 对话框

class PickPointCloudModeDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(PickPointCloudModeDialog)

public:
	PickPointCloudModeDialog();
	virtual ~PickPointCloudModeDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_PICK_POINTCLOUD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	Controller* ctrl;
	afx_msg void OnBnClickedButtonPickPointcloud();
	float m_triangleArea;
	afx_msg void OnBnClickedButtonTriarea();
};
