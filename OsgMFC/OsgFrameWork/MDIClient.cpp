// MDIClient.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OsgFrameWork.h"
#include "MDIClient.h"


// CMDIClient

IMPLEMENT_DYNAMIC(CMDIClient, CWnd)

CMDIClient::CMDIClient()
{

}

CMDIClient::~CMDIClient()
{
}


BEGIN_MESSAGE_MAP(CMDIClient, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMDIClient ��Ϣ�������



void CMDIClient::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

	CDC		*pDC = GetDC();   //�����Լ�ʹ��GetDC()����Ҫ��CPaintDC��  
	CRect	ClientRect;  
	GetClientRect(ClientRect);  
/*
	pDC->FillSolidRect(ClientRect, GetSysColor(COLOR_APPWORKSPACE));
	//����������ʾ������ʾλͼһ����  
	CFont	font;
	font.CreatePointFont(500,   "����");  
	CFont	*fontOld   =   pDC->SelectObject(&font);  
	pDC->SetBkMode(TRANSPARENT);  
	pDC->SetTextAlign(TA_BOTTOM);  
	pDC->SetTextColor(RGB(200,   200,   200));  
	pDC->TextOut(ClientRect.left   +   1,   ClientRect.bottom,   "�ͻ���LOGO",   10);  
	pDC->SetTextColor(RGB(100,   100,   100));  
	pDC->TextOut(ClientRect.left,   ClientRect.bottom   -   1,   "�ͻ���LOGO",   10);  
	pDC->SelectObject(fontOld);  
	font.DeleteObject();  
	ReleaseDC(pDC);   */
	CBitmap   bmp;  
	if(bmp.LoadBitmap(IDB_BITMAP_BG))  
	{  
		BITMAP	bmpInfo;  
		bmp.GetBitmap(&bmpInfo);  
		CDC	dcMemory;  
		dcMemory.CreateCompatibleDC(pDC);
		dcMemory.SelectObject(&bmp);
		pDC->SetStretchBltMode(BLACKONWHITE);
		pDC->StretchBlt(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height(),
			&dcMemory,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);
		bmp.Detach();
	}
}
