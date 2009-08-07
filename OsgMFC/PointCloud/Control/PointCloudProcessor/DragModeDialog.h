#pragma once

#include "resource.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
// DragModeDialog 对话框

class DragModeDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(DragModeDialog)

public:
	DragModeDialog();
	virtual ~DragModeDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_DRAG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	Controller* ctrl;
	afx_msg void OnBnClickedButtonDrag();
	afx_msg void OnBnClickedButtonUndo();
	afx_msg void OnBnClickedButtonRedo();
};
