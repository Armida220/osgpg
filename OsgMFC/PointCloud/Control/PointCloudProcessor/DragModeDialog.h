#pragma once

#include "resource.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
// DragModeDialog �Ի���

class DragModeDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(DragModeDialog)

public:
	DragModeDialog();
	virtual ~DragModeDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_DRAG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	Controller* ctrl;
	afx_msg void OnBnClickedButtonDrag();
	afx_msg void OnBnClickedButtonUndo();
	afx_msg void OnBnClickedButtonRedo();
};
