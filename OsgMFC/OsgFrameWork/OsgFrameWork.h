// OsgFrameWork.h : OsgFrameWork Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// COsgFrameWorkApp:
// �йش����ʵ�֣������ OsgFrameWork.cpp
//

class COsgFrameWorkApp : public CWinApp
{
public:
	COsgFrameWorkApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
};

extern COsgFrameWorkApp theApp;