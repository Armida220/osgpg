// ModelSimplifyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ModelSimplifyApp.h"
#include "ModelSimplifyDlg.h"
#include "ModelSimplifier.h"
#include "LODSimplifier.h"
#include "TriangleSimplifier.h"
#include "TriStripSimplifier.h" //FC
#include "osgDB/filenameutils"

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	WM_THREAD_MESSAGE	(WM_USER + 100)

class CharForLocal
{
public:
	CharForLocal(void)
	{
		setlocale(LC_ALL,"chs");
	}
};

static CharForLocal _CharForLocal;


const char *LOCAL_2_MCBS(LPCTSTR lpstr)
{
#ifdef _UNICODE
	const size_t len = wcslen(lpstr);
	static std::string strRet;
	strRet.resize(len + len + 2, '\0');
	wcstombs(&*strRet.begin(), lpstr, strRet.length());
	return strRet.c_str();
#else
	return lpstr;
#endif
}


LPCTSTR MCBS_2_LOCAL(const char *lpstr)
{
#ifdef _UNICODE
	std::string str;
	const size_t len = strlen(lpstr);
	static std::wstring strRet;
	strRet.resize(len + 2, 0);
	mbstowcs(&*strRet.begin(), lpstr, strRet.length());
	return strRet.c_str();
#else
	return lpstr;
#endif
}


void RecursionFiles(LPCTSTR lpszRecursionFrom, LPCTSTR lpszFileExt, std::list<CString> &listResult, int nRecDepth)
{
	if(!::PathIsDirectory(lpszRecursionFrom))
	{
		return;
	}

	CString strRecursionFrom(lpszRecursionFrom);
	strRecursionFrom.Trim();
	strRecursionFrom.Replace('/', '\\');
	const int nLen = strRecursionFrom.GetLength();
	if(strRecursionFrom[nLen - 1] != '\\')
	{
		strRecursionFrom += '\\';
	}

	const bool bFindAll = (CString::StringLength(lpszFileExt) <= 0 || CString(lpszFileExt) == _T(".*"));
	const CString strFind = strRecursionFrom + _T("*.*");
	CFileFind finder;
	if(!finder.FindFile(strFind, 0))
	{
		return;
	}

	BOOL bContinueFind = TRUE;
	do{
		bContinueFind = finder.FindNextFile();

		if(finder.IsDots())		continue;

		const CString strFilePath = finder.GetFilePath();
		if(finder.IsDirectory() && nRecDepth != 0)
		{
			RecursionFiles(strFilePath, lpszFileExt, listResult, nRecDepth - 1);
			continue;
		}

		if(bFindAll)
		{
			listResult.push_back(strFilePath);
		}
		else
		{
			const CString &strExt = ::PathFindExtension(strFilePath);
			if(!strExt.CompareNoCase(lpszFileExt))
			{
				listResult.push_back(strFilePath);
			}
		}
	}while(bContinueFind);

	finder.Close();
}


class CAboutDlg : public CDialog
{
protected:
	DECLARE_MESSAGE_MAP()

public:
	CAboutDlg();
	enum tagDialogID{ IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange *pDX);

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


CModelSimplifyDlg::CModelSimplifyDlg(CWnd *pParent /*=NULL*/)
	: CDialog(CModelSimplifyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CModelSimplifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_FILES, m_ctrlFileList);
	DDX_Control(pDX, IDC_PROGRESS_CONVERT, m_ctrlProgress);

	DDX_Check(pDX, IDC_CHK_ENABLE_LOD, m_bEnableLOD);
	DDX_Text(pDX, IDC_EDIT_CENTER_THRESHOLD, m_dblCenterThreshold);
	DDX_Text(pDX, IDC_EDIT_RADIUS_THRESHOLD, m_dblRadiusThreshold);
	DDX_Text(pDX, IDC_EDIT_PROBABILITY, m_nProbability);

	DDX_Check(pDX, IDC_CHK_ENABLE_TRIANGLE, m_bEnableTriangle);
	DDX_Text(pDX, IDC_EDIT_AREA_THRESHOLD, m_dblAreaThreshold);
	DDX_Text(pDX, IDC_EDIT_ANGLE_THRESHOLD, m_dblAngleThreshold);

	DDX_Text(pDX, IDC_STATIC_CURRENT_FILE, m_strCurrentFile);
}


BEGIN_MESSAGE_MAP(CModelSimplifyDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_THREAD_MESSAGE, &CModelSimplifyDlg::OnThreadMessage)
	ON_BN_CLICKED(IDC_BTN_ADD_FILES, &CModelSimplifyDlg::OnBnClickedBtnAddFiles)
	ON_BN_CLICKED(IDC_BTN_ADD_DIR, &CModelSimplifyDlg::OnBnClickedBtnAddDir)
	ON_BN_CLICKED(IDC_BTN_REMOVE_SEL, &CModelSimplifyDlg::OnBnClickedBtnRemoveSel)
	ON_BN_CLICKED(IDC_BTN_CLEAR_ALL, &CModelSimplifyDlg::OnBnClickedBtnClearAll)
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CModelSimplifyDlg::OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_CHK_ENABLE_LOD, &CModelSimplifyDlg::OnBnClickedChkEnableLod)
	ON_BN_CLICKED(IDC_CHK_ENABLE_TRIANGLE, &CModelSimplifyDlg::OnBnClickedChkEnableTriangle)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILES, &CModelSimplifyDlg::OnNMClickListFiles)
END_MESSAGE_MAP()


BOOL CModelSimplifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_bEnableLOD = TRUE;
	m_nProbability = 50;
	m_dblCenterThreshold = 5.0;
	m_dblRadiusThreshold = 10.0;

	m_bEnableTriangle = TRUE;
	m_dblAreaThreshold = 10.0;
	m_dblAngleThreshold = 10.0;

	m_strCurrentFile = _T("");
	UpdateData(FALSE);

	EnableFileButtons();
	EnableLOD();
	EnableTriangle();

	CRect rect;
	m_ctrlFileList.GetClientRect(&rect);
	m_ctrlFileList.InsertColumn(0, _T("�ļ�"), LVCFMT_CENTER, rect.Width());

	m_hConvertThread = NULL;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CModelSimplifyDlg::OnCancel()
{
	if(AllowCloseWindow())
	{
		CDialog::OnOK();
	}
}


void CModelSimplifyDlg::OnOK()
{
	if(AllowCloseWindow())
	{
		CDialog::OnOK();
	}
}


bool CModelSimplifyDlg::AllowCloseWindow(void)
{
	if(m_hConvertThread)
	{
		const int nRet = MessageBox(_T("ת���������ڽ����У���Ҫ��ֹô��"), _T("����"), MB_YESNO | MB_ICONQUESTION);
		if(nRet != IDYES)	return false;
		CancelConvert();
	}
	return true;
}


void CModelSimplifyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CModelSimplifyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


HCURSOR CModelSimplifyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CModelSimplifyDlg::EnableLOD(BOOL bForce)
{
	CWnd *pWnd = NULL;

	pWnd = GetDlgItem(IDC_EDIT_CENTER_THRESHOLD);
	pWnd->EnableWindow(!bForce && m_bEnableLOD);

	pWnd = GetDlgItem(IDC_EDIT_RADIUS_THRESHOLD);
	pWnd->EnableWindow(!bForce && m_bEnableLOD);

	pWnd = GetDlgItem(IDC_EDIT_PROBABILITY);
	pWnd->EnableWindow(!bForce && m_bEnableLOD);
}


void CModelSimplifyDlg::EnableTriangle(BOOL bForce)
{
	CWnd *pWnd = NULL;

	pWnd = GetDlgItem(IDC_EDIT_AREA_THRESHOLD);
	pWnd->EnableWindow(!bForce && m_bEnableTriangle);

	pWnd = GetDlgItem(IDC_EDIT_ANGLE_THRESHOLD);
	pWnd->EnableWindow(!bForce && m_bEnableTriangle);
}


void CModelSimplifyDlg::EnableFileButtons(BOOL bForce)
{
	CWnd *pWnd = NULL;

	pWnd = GetDlgItem(IDC_BTN_CLEAR_ALL);
	const int nItemCount = m_ctrlFileList.GetItemCount();
	pWnd->EnableWindow(!bForce && nItemCount > 0);

	pWnd = GetDlgItem(IDC_BTN_BEGIN);
	pWnd->EnableWindow(m_hConvertThread || (!bForce && (m_bEnableTriangle || m_bEnableLOD) && (nItemCount > 0)));

	pWnd = GetDlgItem(IDC_BTN_REMOVE_SEL);
	const UINT nSelItemCount = m_ctrlFileList.GetSelectedCount();
	pWnd->EnableWindow(!bForce && nSelItemCount > 0);
}


void CModelSimplifyDlg::OnBnClickedBtnAddFiles()
{
	const CString strFilter = _T("All Support file(*.ive;*.osg;*.vtst)|*.ive;*.osg;*.vtst|All File(*.*)|*.*||");
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST, strFilter);

	TCHAR *szBuffer = new TCHAR[300000];
	szBuffer[0] = 0;
	dlgFile.m_ofn.nMaxFile = 10000;
	dlgFile.m_ofn.lpstrFile = szBuffer;
	if(dlgFile.DoModal() != IDOK)
	{
		delete[] szBuffer;
		return;
	}

	int nIndex = m_ctrlFileList.GetItemCount();
	POSITION pos = dlgFile.GetStartPosition();
	while(pos)
	{
		const CString strFilePath = dlgFile.GetNextPathName(pos);
		m_ctrlFileList.InsertItem(nIndex, strFilePath);
		++nIndex;
	}
	delete[] szBuffer;
	EnableFileButtons();
}


void CModelSimplifyDlg::OnBnClickedBtnAddDir()
{
	TCHAR		szFileFolder[MAX_PATH] = {0};

	BROWSEINFO	BrowserInfo;
	ZeroMemory(&BrowserInfo, sizeof(BROWSEINFO));
	BrowserInfo.hwndOwner		= GetSafeHwnd();
	BrowserInfo.pszDisplayName	= szFileFolder;
	BrowserInfo.lpszTitle		= _T("ѡ��һ����������vtst�ļ����ļ��У�");
	BrowserInfo.ulFlags			= BIF_EDITBOX;

	LPITEMIDLIST pItemID_List = SHBrowseForFolder(&BrowserInfo);
	if(!pItemID_List)	return;

	std::list<CString>	listRecursionResult;
	SHGetPathFromIDList(pItemID_List, szFileFolder);
	::RecursionFiles(szFileFolder, _T(".vtst"), listRecursionResult, -1);

	int nIndex = m_ctrlFileList.GetItemCount();
	std::list<CString>::const_iterator itorFile = listRecursionResult.begin();
	while(itorFile != listRecursionResult.end())
	{
		const CString &strFilePath = *itorFile;

		m_ctrlFileList.InsertItem(nIndex, strFilePath);

		++nIndex;	++itorFile;
	}
	EnableFileButtons();
}


void CModelSimplifyDlg::OnBnClickedBtnRemoveSel()
{
	const int nItemCount = m_ctrlFileList.GetItemCount();
	for(int nIndex = nItemCount - 1; nIndex >= 0; nIndex--)
	{
		const UINT uState = m_ctrlFileList.GetItemState(nIndex, LVIS_SELECTED);
		if(uState == LVIS_SELECTED)
		{
			m_ctrlFileList.DeleteItem(nIndex);
		}
	}
	EnableFileButtons();
}


void CModelSimplifyDlg::OnBnClickedBtnClearAll()
{
	m_ctrlFileList.DeleteAllItems();
}


void CModelSimplifyDlg::OnBnClickedBtnBegin()
{
	if(m_hConvertThread)
	{
		CancelConvert();
	}
	else
	{
		BeginConvert();
	}
}


void CModelSimplifyDlg::OnBnClickedChkEnableLod()
{
	UpdateData(TRUE);
	EnableLOD();
	EnableFileButtons();
}


void CModelSimplifyDlg::OnBnClickedChkEnableTriangle()
{
	UpdateData(TRUE);
	EnableTriangle();
	EnableFileButtons();
}

void CModelSimplifyDlg::OnNMClickListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	EnableFileButtons();
	*pResult = 0;
}


void CModelSimplifyDlg::BeginConvert(void)
{
	// 1�����������ļ�
	m_vecFileForConvert.clear();

	const int nItemCount = m_ctrlFileList.GetItemCount();
	for(int x = 0; x < nItemCount; x++)
	{
		std::string strFilePath;

		const CString strItemText = m_ctrlFileList.GetItemText(x, 0);
		const CString strFileExt = ::PathFindExtension(strItemText.GetString());
		if(strFileExt.CompareNoCase(_T(".vtst")) == 0)
		{
			TCHAR	szItemText[MAX_PATH];
			::StrCpy(szItemText, strItemText.GetString());
			::PathRemoveFileSpec(szItemText);

			strFilePath = ::LOCAL_2_MCBS(szItemText);
			strFilePath += "\\main.ive";
		}
		else
		{
			strFilePath = ::LOCAL_2_MCBS(strItemText.GetString());
		}
		m_vecFileForConvert.push_back(strFilePath);
	}


	// 2��һЩ��ʼ������
	m_ctrlProgress.SetRange(0, (short)m_vecFileForConvert.size());
	m_ctrlProgress.SetStep(1);


	// 3������ת���߳�
	m_bUserCanceled = false;
	m_hConvertThread = (HANDLE)::_beginthreadex(NULL, 0, CModelSimplifyDlg::ConvertThreadProc, (void *)this, 0, NULL);
	if(m_hConvertThread)
	{
		CWnd *pWnd = GetDlgItem(IDC_BTN_BEGIN);
		pWnd->SetWindowText(_T("ȡ���򻯽���"));

		EnableFileButtons(TRUE);
		EnableLOD(TRUE);
		EnableTriangle(TRUE);
	}
	else
	{
		MessageBox(_T("�޷�ִ�м�����"), _T("����"), MB_OK | MB_ICONERROR);
	}
}


void CModelSimplifyDlg::CancelConvert(void)
{
	if(m_hConvertThread)
	{
		CWaitCursor wait;
		m_bUserCanceled = true;
		::WaitForSingleObject(m_hConvertThread, INFINITE);
		m_hConvertThread = NULL;
	}

	EnableFileButtons();
	EnableLOD();
	EnableTriangle();
}


LRESULT CModelSimplifyDlg::OnThreadMessage(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 0)
	{
		// ������Ϣ��֪ͨһ������ת�����ڼ����ļ���

		const size_t nIndex = (size_t)lParam;
		const std::string strText = m_vecFileForConvert.at(nIndex);
		m_strCurrentFile = ::MCBS_2_LOCAL(strText.c_str());

		m_ctrlProgress.StepIt();

		UpdateData(FALSE);
	}
	else if(wParam == 1)
	{
		// ת���ɹ����
		MessageBox(_T("ת�����"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);

		m_ctrlProgress.SetPos(0);
		m_strCurrentFile.Empty();
		UpdateData(FALSE);

		EnableFileButtons();
		EnableLOD();
		EnableTriangle();

		CWnd *pWnd = GetDlgItem(IDC_BTN_BEGIN);
		pWnd->SetWindowText(_T("��ʼִ�м�"));

		m_hConvertThread = NULL;
	}
	else if(wParam == 2)
	{
		// �û�ȡ��
		m_ctrlProgress.SetPos(0);
		m_strCurrentFile.Empty();
		UpdateData(FALSE);

		EnableFileButtons();
		EnableLOD();
		EnableTriangle();

		CWnd *pWnd = GetDlgItem(IDC_BTN_BEGIN);
		pWnd->SetWindowText(_T("��ʼִ�м�"));

		m_hConvertThread = NULL;
	}
	else
	{
		// δ֪�Ĵ���
		ASSERT(FALSE);
	}
	return 0L;
}


unsigned _stdcall CModelSimplifyDlg::ConvertThreadProc(void *pVoid)
{
	CModelSimplifyDlg *pThis = (CModelSimplifyDlg *)pVoid;

	ModelSimplifier  convert;
	//FC ע������һ��Ҫע��򻯵�˳��
	//���ȼ�main�ļ���LODSimplifier��
	//��μ������棨TriangleSimplifier��
	//��������������TriStripSimplifier��
	if(pThis->m_bEnableLOD)
	{
		LODSimplifier *pSimplifier = new LODSimplifier;
		pSimplifier->SetThreshold(pThis->m_nProbability / 100.0, pThis->m_dblCenterThreshold, pThis->m_dblRadiusThreshold);
		convert.AddSimplifyFunctor(pSimplifier);
	}

	if(pThis->m_bEnableTriangle)
	{
		TriangleSimplifier *pSimplifier = new TriangleSimplifier;
			pSimplifier->SetThreshold(pThis->m_dblAreaThreshold, sin(pThis->m_dblAngleThreshold * osg::PI / 180.0));
		convert.AddSimplifyFunctor(pSimplifier);
	}

	//FC {
	{
		TriStripSimplifier *pSimplifier = new TriStripSimplifier;
		convert.AddSimplifyFunctor(pSimplifier);
	}
	//FC }

	std::vector<std::string>::const_iterator itorFilePath = pThis->m_vecFileForConvert.begin();
	while(itorFilePath != pThis->m_vecFileForConvert.end())
	{
		if(pThis->m_bUserCanceled)	break;

		const size_t nIndex = itorFilePath - pThis->m_vecFileForConvert.begin();
		pThis->PostMessage(WM_THREAD_MESSAGE, 0, (LPARAM)nIndex);

		const std::string strFilePath = *itorFilePath;

		//FC {
		std::string dbPath = osgDB::getFilePath(strFilePath);
		::SetCurrentDirectoryA(dbPath.c_str());
		//FC }

		osg::ref_ptr<osg::Node> pRootNode = osgDB::readNodeFile(strFilePath);
		pRootNode->accept(convert);
		osgDB::writeNodeFile(*pRootNode, strFilePath);

		++itorFilePath;
	}
	pThis->PostMessage(WM_THREAD_MESSAGE, 1);
	return 0;
}
