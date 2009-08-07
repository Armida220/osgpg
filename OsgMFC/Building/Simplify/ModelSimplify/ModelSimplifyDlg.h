// ModelSimplifyDlg.h : 头文件
//

#pragma once


// CModelSimplifyDlg 对话框
class CModelSimplifyDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
public:
	CModelSimplifyDlg(CWnd *pParent = NULL);	// 标准构造函数
	enum tagDialogID{ IDD = IDD_MODELSIMPLIFY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

protected:
	void	EnableFileButtons(BOOL bForce = FALSE);
	void	EnableLOD(BOOL bForce = FALSE);
	void	EnableTriangle(BOOL bForce = FALSE);

	void	BeginConvert(void);
	void	CancelConvert(void);

	bool	AllowCloseWindow(void);

	static unsigned _stdcall ConvertThreadProc(void *pVoid);

protected:
	afx_msg void	OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void	OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void	OnBnClickedBtnAddFiles();
	afx_msg void	OnBnClickedBtnAddDir();
	afx_msg void	OnBnClickedBtnRemoveSel();
	afx_msg void	OnBnClickedBtnClearAll();
	afx_msg void	OnBnClickedBtnBegin();
	afx_msg void	OnBnClickedChkEnableLod();
	afx_msg void	OnBnClickedChkEnableTriangle();
	afx_msg void	OnNMClickListFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT	OnThreadMessage(WPARAM wParam, LPARAM lParam);

protected:
	HICON			m_hIcon;

	CListCtrl		m_ctrlFileList;
	CProgressCtrl	m_ctrlProgress;

	HANDLE				m_hConvertThread;
	volatile bool		m_bUserCanceled;

	BOOL		m_bEnableLOD;
	double		m_dblCenterThreshold;
	double		m_dblRadiusThreshold;
	int			m_nProbability;

	BOOL		m_bEnableTriangle;
	double		m_dblAreaThreshold;
	double		m_dblAngleThreshold;

	CString		m_strCurrentFile;

	std::vector<std::string>	m_vecFileForConvert;

};
