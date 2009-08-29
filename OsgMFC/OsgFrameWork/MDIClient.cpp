// MDIClient.cpp : 实现文件
//

#include "stdafx.h"
#include "OsgFrameWork.h"
#include "MDIClient.h"

// CMDIClient

IMPLEMENT_DYNAMIC(CMDIClient, CWnd)

CMDIClient::CMDIClient()
{
	bmp.LoadBitmap(IDB_BITMAP_BG);
}

CMDIClient::~CMDIClient()
{
	bmp.Detach();
}


BEGIN_MESSAGE_MAP(CMDIClient, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMDIClient 消息处理程序



void CMDIClient::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CDC		*pDC = GetDC();   //必须自己使用GetDC()，不要用CPaintDC
	CRect	ClientRect;
	GetClientRect(ClientRect);

	
	{
		BITMAP	bmpInfo;
		bmp.GetBitmap(&bmpInfo);
		CDC	dcMemory;
		dcMemory.CreateCompatibleDC(pDC);
		dcMemory.SelectObject(&bmp);
		pDC->SetStretchBltMode(BLACKONWHITE);
		pDC->StretchBlt(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height(),
			&dcMemory,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);
	}

	dirty=true;
	UpdateConsoleOuts();
}

void CMDIClient::UpdateConsoleOuts()
{
	if(dirty) {
		dirty = false;

		CDC		*pDC = GetDC();
		CRect	ClientRect;  
		GetClientRect(ClientRect);  
		CFont	font;
		int size = 150;
		font.CreatePointFont(size,   "Consolas");
		CFont	*fontOld   =   pDC->SelectObject(&font);  
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextAlign(TA_BOTTOM);
		pDC->SetTextColor(RGB(0, 255, 0));
		
		for(int i=0; i<consoleOuts.size(); ++i) {
			pDC->TextOut(ClientRect.left + 5,
				ClientRect.top + 18*(i+1)+5,   CString(consoleOuts.at(i).c_str()));
		}

		pDC->SelectObject(fontOld);
		font.DeleteObject();
		ReleaseDC(pDC);
	}
}