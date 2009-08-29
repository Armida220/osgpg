#pragma once


// CMDIClient
#include <string>
#include <vector>
#include <queue>
using namespace std;

class CMDIClient : public CWnd
{
	DECLARE_DYNAMIC(CMDIClient)

public:
	CMDIClient();
	virtual ~CMDIClient();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CBitmap   bmp;

	afx_msg void OnPaint();

	vector<string> consoleOuts;
	bool dirty;

	void UpdateConsoleOuts();
};