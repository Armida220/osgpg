#pragma once
#include "Resource.h"

// ImgMountainParaSetter �Ի���

class ImgMountainParaSetter : public CDialog
{
	DECLARE_DYNAMIC(ImgMountainParaSetter)

public:
	ImgMountainParaSetter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ImgMountainParaSetter();

// �Ի�������
	enum { IDD = IDD_DIALOG_PARASETTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_heightScale;
	afx_msg void OnBnClickedOk();
	double m_planescale;
};
