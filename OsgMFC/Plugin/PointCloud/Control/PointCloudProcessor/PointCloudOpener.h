#pragma once

#include "resource.h"
// PointCloudOpener 对话框

class PointCloudOpener : public CDialog
{
	DECLARE_DYNAMIC(PointCloudOpener)

public:
	PointCloudOpener(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PointCloudOpener();

// 对话框数据
	enum { IDD = IDD_DIALOG_OPEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFileName;
	int			m_iPointCloudChoice;
	int			m_iTriangleChoice;
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioPointcloudNone();
	afx_msg void OnBnClickedRadioPointcloudRaw();
	afx_msg void OnBnClickedRadioPointcloudKd();
	afx_msg void OnBnClickedRadioPointcloudOsg();
	afx_msg void OnBnClickedRadioTriangleNone();
	afx_msg void OnBnClickedRadioTriangleRaw();
	afx_msg void OnBnClickedRadioTrianglePly();
	afx_msg void OnBnClickedRadioTriangleOsg();
};
