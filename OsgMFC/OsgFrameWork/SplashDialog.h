#pragma once


// CSplashDialog �Ի���

class CSplashDialog : public CDialog
{
	DECLARE_DYNAMIC(CSplashDialog)

public:
	CSplashDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSplashDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_SPLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
