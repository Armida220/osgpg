#pragma once
#include "Resource.h"

// ImgMountainParaSetter 对话框

class ImgMountainParaSetter : public CDialog
{
	DECLARE_DYNAMIC(ImgMountainParaSetter)

public:
	ImgMountainParaSetter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ImgMountainParaSetter();

// 对话框数据
	enum { IDD = IDD_DIALOG_PARASETTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_heightScale;
	afx_msg void OnBnClickedOk();
	double m_planescale;
};
