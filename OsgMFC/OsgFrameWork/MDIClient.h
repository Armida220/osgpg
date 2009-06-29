#pragma once


// CMDIClient

class CMDIClient : public CWnd
{
	DECLARE_DYNAMIC(CMDIClient)

public:
	CMDIClient();
	virtual ~CMDIClient();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


