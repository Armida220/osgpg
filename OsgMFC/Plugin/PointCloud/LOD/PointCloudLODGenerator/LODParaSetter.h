#pragma once


#include "resource.h"
// LODParaSetter 对话框

class LODParaSetter : public CDialog
{
	DECLARE_DYNAMIC(LODParaSetter)

public:
	LODParaSetter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LODParaSetter();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOD_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_sampleRatio;
	UINT m_targetNumOnLeaf;
	afx_msg void OnBnClickedOk();
};
