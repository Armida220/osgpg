// ImgMeasurementDlg.h : ͷ�ļ�
//

#pragma once

// CImgMeasurementDlg �Ի���
#include "ximage.h"
#include <vector>

using namespace std;

class CMyPoint : public CObject
{
public:
	long x;
	long y;
};

template<class Type>
struct ImagePoint {
	ImagePoint(Type x_=0, Type y_=0) { x=x_; y=y_; }
	Type x,y;
};
typedef vector<ImagePoint<long>> ImagePointArr;

class CImgMeasurementDlg : public CDialog
{
// ����
public:
	CImgMeasurementDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IMGMEASUREMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CxImage m_image;
	CxImage m_zoomRectImage;
	BOOL    m_bShowZoomRectImage;
	CPoint  m_zoomCenterPt;
	CObArray m_ptArray;

	void DrawPt(CPaintDC *pDC,CPoint pointInImage, CString labelStr, COLORREF crsColor,int bold = 1);
	CxImage   GetWindowImageFromWholeImage( CxImage* wholeImage,
		CPoint   centerPoint,
		CRect    windowRect);
	CString FindExtension(const CString& name);
	int FindType(const CString& ext);



	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void GetImagePointArray(ImagePointArr& ipa);
};
