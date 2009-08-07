#pragma once

#include "resource.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
// PickPointCloudModeDialog �Ի���

class PickPointCloudModeDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(PickPointCloudModeDialog)

public:
	PickPointCloudModeDialog();
	virtual ~PickPointCloudModeDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_PICK_POINTCLOUD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	Controller* ctrl;
	afx_msg void OnBnClickedButtonPickPointcloud();
	float m_triangleArea;
	afx_msg void OnBnClickedButtonTriarea();
};
