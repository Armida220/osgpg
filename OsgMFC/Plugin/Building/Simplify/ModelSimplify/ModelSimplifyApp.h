// ModelSimplify.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


class CModelSimplifyApp : public CWinApp
{
protected:
	DECLARE_MESSAGE_MAP()

public:
	CModelSimplifyApp();

public:
	virtual BOOL InitInstance();
};

extern CModelSimplifyApp theApp;