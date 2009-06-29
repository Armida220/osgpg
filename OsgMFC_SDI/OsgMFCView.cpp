// OsgMFCView.cpp : COsgMFCView ���ʵ��
//

#include "stdafx.h"
#include "OsgMFC.h"

#include "OsgMFCDoc.h"
#include "OsgMFCView.h"

#include "WorkFlowPluginBase.h"

//#include <osgDB/ReadFile>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COsgMFCView

IMPLEMENT_DYNCREATE(COsgMFCView, CView)

BEGIN_MESSAGE_MAP(COsgMFCView, CView)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_OSG_RUN, &COsgMFCView::OnOsgRun)
	ON_COMMAND(ID_LOAD_WORKFLOW_PLUGIN, &COsgMFCView::OnLoadWorkflowPlugin)
	ON_COMMAND(ID_OSG_STOP, &COsgMFCView::OnOsgStop)
END_MESSAGE_MAP()

// COsgMFCView ����/����

COsgMFCView::COsgMFCView()
{
	m_Closed_yet=true;
}

COsgMFCView::~COsgMFCView()
{
}

BOOL COsgMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// COsgMFCView ����

void COsgMFCView::OnDraw(CDC* /*pDC*/)
{
	COsgMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// COsgMFCView ���

#ifdef _DEBUG
void COsgMFCView::AssertValid() const
{
	CView::AssertValid();
}

void COsgMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COsgMFCDoc* COsgMFCView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COsgMFCDoc)));
	return (COsgMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// COsgMFCView ��Ϣ�������

void COsgMFCView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CRect rect;
	GetClientRect(&rect);

	m_frame.SetupWindow(GetSafeHwnd(), rect);
}

void COsgMFCView::OnClose()
{
	m_frame.Stop();
	m_frame.ReleaseImpl();

	CView::OnClose();
}

void COsgMFCView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}

void COsgMFCView::OnOsgRun()
{
	if(!m_Closed_yet) {
		AfxMessageBox("����ֹͣ��ǰ�������������¿�ʼ��");
		return;
	}
	m_Closed_yet = false;

	COsgMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	FC::WorkFlowPluginBase* workFlow =
		m_pluginManager.GetWorkFlow(0);
	if(workFlow) {
		osg::ref_ptr<osg::Node> node = 
			workFlow->CreateSceneData();
		
		//ATTENTION! can not use m_frame.GetViewer()->setSceneData(), 
		//for the m_frame will eventually setSceneData() at Run()
		m_frame.Load(node);
	}

	m_frame.Run();
}

void COsgMFCView::OnLoadWorkflowPlugin()
{
	CFileDialog dlg(TRUE, NULL, NULL, 0, "DLL�ļ�(*.dll)|*.dll");
	if(dlg.DoModal()!=IDOK) {
		return;
	}
	CString strDllFileName = dlg.GetPathName();

	if( m_pluginManager.AddPlugin(strDllFileName) )
	{
		AfxMessageBox("�ɹ����ز����");
	}
}

void COsgMFCView::OnOsgStop()
{
	m_frame.Stop();
	m_Closed_yet = true;
	Invalidate(true);
}
