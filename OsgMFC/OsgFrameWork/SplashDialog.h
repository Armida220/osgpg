#pragma once


// CSplashDialog 对话框

class CSplashDialog : public CDialog
{
	DECLARE_DYNAMIC(CSplashDialog)

public:
	CSplashDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSplashDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_SPLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
