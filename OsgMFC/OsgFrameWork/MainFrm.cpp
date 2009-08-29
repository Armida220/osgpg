// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "OsgFrameWork.h"

#include "MainFrm.h"
#include "SplashDialog.h"

#include "Console.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//��ӭ����
	CSplashDialog *dlg = new CSplashDialog(this);
	dlg->Create(CSplashDialog::IDD,this); //�����Ի���
	dlg->ShowWindow(SW_SHOW); //��ʾ�Ի���
	dlg->UpdateWindow();
	Sleep(500); //������ʾͣ��ʱ�䣬��λΪ���� 

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	dlg->DestroyWindow(); //���ٶԻ���

	this->SetTimer(0, 5000, NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


BOOL CMainFrame::CreateClient(LPCREATESTRUCT   lpCreateStruct,   CMenu*   pWindowMenu)
{
	if(!CMDIFrameWnd::CreateClient(lpCreateStruct,   pWindowMenu))  
		return   FALSE;  
	if(!m_wndMDIClient.SubclassWindow(m_hWndMDIClient))  
		return   FALSE;

	return   TRUE;
}

void CMainFrame::OnDestroy()
{
	m_wndMDIClient.UnsubclassWindow();
	CMDIFrameWnd::OnDestroy();
}
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
#define FC_TESTING 1
#if FC_TESTING
	if(nIDEvent==0) {
		HANDLE hStdout; 
		SMALL_RECT srctReadRect; 
		CHAR_INFO* chiBuffer=0;
		COORD coordBufSize; 
		COORD coordBufCoord;
		BOOL fSuccess;

		hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
		if (hStdout == INVALID_HANDLE_VALUE) 
		{
			printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError()); 
			return;
		}
		
		CONSOLE_SCREEN_BUFFER_INFO screen;
		::GetConsoleScreenBufferInfo(hStdout, &screen);

		srctReadRect = screen.srWindow;

		coordBufSize.Y = srctReadRect.Bottom + 1;
		coordBufSize.X = srctReadRect.Right + 1;

		chiBuffer = new CHAR_INFO[coordBufSize.Y * coordBufSize.X];

		coordBufCoord.X = 0; 
		coordBufCoord.Y = 0; 

		// Copy the block from the screen buffer to the temp. buffer. 
		fSuccess = ReadConsoleOutput( 
			hStdout,        // screen buffer to read from 
			chiBuffer,      // buffer to copy into 
			coordBufSize,   // col-row size of chiBuffer 
			coordBufCoord,  // top left dest. cell in chiBuffer 
			&srctReadRect); // screen buffer source rectangle 
		if (! fSuccess) 
		{
			printf("ReadConsoleOutput failed - (%d)\n", GetLastError()); 
			return;
		}

		m_wndMDIClient.consoleOuts.clear();
		for(int i=0; i<coordBufSize.Y; ++i) {
			string line;
			for (int j=0; j<coordBufSize.X; ++j) {
				line.push_back( chiBuffer[i*coordBufSize.X+j].Char.AsciiChar );
			}
			m_wndMDIClient.consoleOuts.push_back(line);
		}
		m_wndMDIClient.dirty = true;
		m_wndMDIClient.UpdateConsoleOuts();

		if(chiBuffer) delete [] chiBuffer;
	}
#endif
	CMDIFrameWnd::OnTimer(nIDEvent);
}
