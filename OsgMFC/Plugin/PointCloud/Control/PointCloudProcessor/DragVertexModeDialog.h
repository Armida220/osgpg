#pragma once

#include "resource.h"
#include "../PointCloudProcess/PointCloudProcess/Controller.h"
// DragVertexModeDialog �Ի���

class DragVertexModeDialog : public CPropertyPage
{
	DECLARE_DYNAMIC(DragVertexModeDialog)

public:
	DragVertexModeDialog();
	virtual ~DragVertexModeDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_DRAGVERTEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	Controller* ctrl;
	afx_msg void OnBnClickedButtonDragvertex();
};
