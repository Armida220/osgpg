// OsgMFC.h : OsgMFC Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// COsgMFCApp:
// �йش����ʵ�֣������ OsgMFC.cpp
//

class COsgMFCApp : public CWinApp
{
public:
	COsgMFCApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COsgMFCApp theApp;