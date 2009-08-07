#pragma once

#include "resource.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
// PickTriangleModeDialog 对话框

class PickTriangleModeDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(PickTriangleModeDialog)

public:
	PickTriangleModeDialog();
	virtual ~PickTriangleModeDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_PICK_TRIANGLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	Controller* ctrl;
	afx_msg void OnBnClickedButtonPickTriangle();
	float m_ax;
	float m_ay;
	float m_az;
	float m_bx;
	float m_by;
	float m_bz;
	float m_cx;
	float m_cy;
	float m_cz;
	afx_msg void OnBnClickedButtonCurTriangle();
};
