// ImgMeasurementDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "ImgMeasurementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgMeasurementDlg �Ի���




CImgMeasurementDlg::CImgMeasurementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImgMeasurementDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bShowZoomRectImage = FALSE;
}

void CImgMeasurementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImgMeasurementDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CImgMeasurementDlg ��Ϣ�������

BOOL CImgMeasurementDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
// 	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
// 	ASSERT(IDM_ABOUTBOX < 0xF000);

// 	CMenu* pSysMenu = GetSystemMenu(FALSE);
// 	if (pSysMenu != NULL)
// 	{
// 		CString strAboutMenu;
// 		strAboutMenu.LoadString(IDS_ABOUTBOX);
// 		if (!strAboutMenu.IsEmpty())
// 		{
// 			pSysMenu->AppendMenu(MF_SEPARATOR);
// 			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
// 		}
// 	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrTitle="�ҵ��ļ��򿪶Ի���";
	fileDlg.m_ofn.lpstrFilter="Text Files(*.JPG)\0*.JPG\0All Files(*.*)\0*.*\0\0";

	if(IDOK==fileDlg.DoModal())
	{
		CString strExt = FindExtension(fileDlg.GetPathName());
		int type = FindType(strExt);

		m_image.Load(fileDlg.GetPathName(),type);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CImgMeasurementDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
// 	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
// 	{
// 		CAboutDlg dlgAbout;
// 		dlgAbout.DoModal();
// 	}
// 	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImgMeasurementDlg::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	m_image.Draw(dc.GetSafeHdc(),rect);
	
	int ptCount = m_ptArray.GetSize();
	for(int i=0;i<ptCount;i++)
	{
		CMyPoint *pMyPoint = (CMyPoint *)m_ptArray.GetAt(i);
		CPoint pt;
		pt.x = (long)(pMyPoint->x*(rect.Width()/(m_image.GetWidth()*1.0)));
		pt.y = (long)(pMyPoint->y*(rect.Height()/(m_image.GetHeight()*1.0)));

		CString labelStr; labelStr.Format("%d", i);
		DrawPt(&dc,pt, labelStr, RGB(255,0,0));

	}

	if(m_bShowZoomRectImage)
	{
		m_zoomRectImage.Draw(dc.GetSafeHdc(),CRect( m_zoomCenterPt.x-120,
			m_zoomCenterPt.y-120,
			m_zoomCenterPt.x+120,
			m_zoomCenterPt.y+120));
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CImgMeasurementDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImgMeasurementDlg::DrawPt(CPaintDC *pDC,CPoint pointInImage, CString labelStr, COLORREF crsColor,int bold)//��Ļ����ϵ
{
	CPen PenCross(PS_SOLID, bold, crsColor );
	CPen *oldPen;
	oldPen=pDC->SelectObject(&PenCross);

	pDC->MoveTo(pointInImage.x-7, pointInImage.y);
	pDC->LineTo(pointInImage.x+7, pointInImage.y);

	pDC->MoveTo(pointInImage.x, pointInImage.y-7);
	pDC->LineTo(pointInImage.x, pointInImage.y+7);

	pDC->TextOut(pointInImage.x, pointInImage.y, labelStr);

	pDC->SelectObject(oldPen);
}

CxImage  CImgMeasurementDlg::GetWindowImageFromWholeImage( CxImage* wholeImage,
												 CPoint   centerPoint,
												 CRect    windowRect)
{
	int wholeImageHeight=wholeImage->GetHeight();
	int wholeImageWidth=wholeImage->GetWidth();
	//...����ƥ��㱣�������ϵ��CxImage����ϵ��ͬ��������������ת��
	CPoint centerPointInCxImageCoordinate;
	centerPointInCxImageCoordinate.x=centerPoint.x;
	centerPointInCxImageCoordinate.y=(wholeImageHeight-1-centerPoint.y);

	CxImage windowImage(windowRect.Width(),windowRect.Height(),24);
	CPoint origin;
	origin.x=centerPointInCxImageCoordinate.x-windowRect.Width()/2;
	origin.y=centerPointInCxImageCoordinate.y-windowRect.Height()/2;
	for (int i=0;i<windowRect.Height();i++)
	{
		for (int j=0;j<windowRect.Width();j++)
		{
			if (origin.x+j<0||origin.x+j>=wholeImageWidth
				||origin.y+i<0||origin.y+i>=wholeImageHeight)
			{
				windowImage.SetPixelColor(j,i,RGB(0,0,0));
			}
			else
			{
				RGBQUAD rgb;
				rgb=wholeImage->GetPixelColor(origin.x+j,origin.y+i);
				windowImage.SetPixelColor(j,i,RGB(rgb.rgbRed,rgb.rgbGreen,
					rgb.rgbBlue));
			}
		}
	}
	for (int i=0;i<5;i++)
	{
		windowImage.SetPixelColor(windowRect.Width()/2-2+i,
			windowRect.Height()/2,
			RGB(0,255,0));
		windowImage.SetPixelColor(windowRect.Width()/2,
			windowRect.Height()/2-2+i,
			RGB(0,255,0));
	}
	return windowImage;
}
void CImgMeasurementDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	CRect rect;
	GetClientRect(&rect);
	CMyPoint pt;
	pt.x = (long)(point.x*(m_image.GetWidth()/(rect.Width()*1.0)));
	pt.y = (long)(point.y*(m_image.GetHeight()/(rect.Height()*1.0)));

	m_zoomRectImage = GetWindowImageFromWholeImage(&m_image,CPoint(pt.x,pt.y),
		CRect(0,0,120,120));
	m_zoomCenterPt = point;
	m_bShowZoomRectImage = TRUE;
	Invalidate();
	CDialog::OnLButtonDown(nFlags, point);
}

void CImgMeasurementDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(&rect);
	m_bShowZoomRectImage = FALSE;
	if( point.x - m_zoomCenterPt.x <120
		&& point.x - m_zoomCenterPt.x >-120
		&& point.y - m_zoomCenterPt.y <120
		&& point.y - m_zoomCenterPt.y >-120)
	{
		CMyPoint *pMyPoint = new CMyPoint();
		pMyPoint->x = m_zoomCenterPt.x*(m_image.GetWidth()/(rect.Width()*1.0)) +
			(point.x - m_zoomCenterPt.x)/2;								 

		pMyPoint->y = m_zoomCenterPt.y*(m_image.GetHeight()/(rect.Height()*1.0)) +
			(point.y - m_zoomCenterPt.y)/2;	
		m_ptArray.Add(pMyPoint);

		//CString str;
		//str.Format("%d,%d",pMyPoint->x,pMyPoint->y);
		//MessageBox(str);

	}
	Invalidate();
	CDialog::OnLButtonUp(nFlags, point);
}

int CImgMeasurementDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	ModifyStyle( NULL, WS_THICKFRAME);
	return 0;
}

CString CImgMeasurementDlg::FindExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(i+1);
		}
	}
	return CString("");
}

int CImgMeasurementDlg::FindType(const CString& ext)
{
	int type = 0;
	if (ext == "bmp")					type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
	else if (ext=="jpg"||ext=="jpeg"||ext=="JPG")	type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
	else if (ext == "gif")				type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
	else if (ext == "png")				type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
	else if (ext=="mng"||ext=="jng")	type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
	else if (ext == "ico")				type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
	else if (ext=="tiff"||ext=="tif")	type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
	else if (ext=="tga")				type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
	else if (ext=="pcx")				type = CXIMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
	else if (ext=="wbmp")				type = CXIMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
	else if (ext=="wmf"||ext=="emf")	type = CXIMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
	else if (ext=="j2k"||ext=="jp2")	type = CXIMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
	else if (ext=="jbg")				type = CXIMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
	else if (ext=="jp2"||ext=="j2k")	type = CXIMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
	else if (ext=="jpc"||ext=="j2c")	type = CXIMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
	else if (ext=="pgx")				type = CXIMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_RAS
	else if (ext=="ras")				type = CXIMAGE_FORMAT_RAS;
#endif

	return type;
}

void CImgMeasurementDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Invalidate();
	// TODO: �ڴ˴������Ϣ����������
}

BOOL CImgMeasurementDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CImgMeasurementDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	CDialog::OnMouseMove(nFlags, point);
}

void CImgMeasurementDlg::GetImagePointArray(ImagePointArr& ipa)
{
	int ptCount = m_ptArray.GetSize();
	for(int i=0;i<ptCount;i++)
	{
		CMyPoint *pMyPoint = (CMyPoint *)m_ptArray.GetAt(i);
		ipa.push_back(ImagePoint<long>(pMyPoint->x, pMyPoint->y));
	}
}