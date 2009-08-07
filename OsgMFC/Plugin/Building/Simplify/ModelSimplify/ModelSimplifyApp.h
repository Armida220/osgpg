// ModelSimplify.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


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