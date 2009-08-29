// MainFrm.cpp : CMainFrame 类的实现
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
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//欢迎画面
	CSplashDialog *dlg = new CSplashDialog(this);
	dlg->Create(CSplashDialog::IDD,this); //创建对话框
	dlg->ShowWindow(SW_SHOW); //显示对话框
	dlg->UpdateWindow();
	Sleep(500); //画面显示停留时间，单位为毫秒 

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	dlg->DestroyWindow(); //销毁对话框

	this->SetTimer(0, 5000, NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

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
