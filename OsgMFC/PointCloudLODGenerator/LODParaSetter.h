#pragma once


#include "resource.h"
// LODParaSetter �Ի���

class LODParaSetter : public CDialog
{
	DECLARE_DYNAMIC(LODParaSetter)

public:
	LODParaSetter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LODParaSetter();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOD_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float m_sampleRatio;
	UINT m_targetNumOnLeaf;
	afx_msg void OnBnClickedOk();
};
