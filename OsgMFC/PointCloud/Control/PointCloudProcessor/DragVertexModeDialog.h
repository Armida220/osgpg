#pragma once

#include "resource.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
// DragVertexModeDialog 对话框

class DragVertexModeDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(DragVertexModeDialog)

public:
	DragVertexModeDialog();
	virtual ~DragVertexModeDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_DRAGVERTEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	Controller* ctrl;
	afx_msg void OnBnClickedButtonDragvertex();
};
