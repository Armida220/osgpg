// OsgFrameWorkView.cpp : COsgFrameWorkView ���ʵ��
//

#include "stdafx.h"
#include "OsgFrameWork.h"

#include "OsgFrameWorkDoc.h"
#include "OsgFrameWorkView.h"

#include "MainFrm.h"

#include <osgGA/GUIEventHandler>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COsgFrameWorkView

IMPLEMENT_DYNCREATE(COsgFrameWorkView, CView)

BEGIN_MESSAGE_MAP(COsgFrameWorkView, CView)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// COsgFrameWorkView ����/����

COsgFrameWorkView::COsgFrameWorkView()
{
	// TODO: �ڴ˴����ӹ������

}

COsgFrameWorkView::~COsgFrameWorkView()
{
}

BOOL COsgFrameWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// COsgFrameWorkView ����

void COsgFrameWorkView::OnDraw(CDC* /*pDC*/)
{
	COsgFrameWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ�����������ӻ��ƴ���
}


// COsgFrameWorkView ���

#ifdef _DEBUG
void COsgFrameWorkView::AssertValid() const
{
	CView::AssertValid();
}

void COsgFrameWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COsgFrameWorkDoc* COsgFrameWorkView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COsgFrameWorkDoc)));
	return (COsgFrameWorkDoc*)m_pDocument;
}
#endif //_DEBUG


// COsgFrameWorkView ��Ϣ��������

void COsgFrameWorkView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CRect rect;
	GetClientRect(&rect);

	m_frame.SetupWindow(GetSafeHwnd(), rect);

	//get main frame
	COsgFrameWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc) {
		FC::PluginManager& pm = 
			pDoc->m_pluginManager;

		FC::WorkFlowPluginBase* workFlow =
			pm.GetWorkFlow(0);

		if(workFlow) { //���ȳ��� ���������
			//����LauncherInfo
			workFlow->SetLauncherInfo(this);
			
			//add scene data
			osg::ref_ptr<osg::Node> node = 
				workFlow->CreateSceneData();
			//ATTENTION! can not use m_frame.GetViewer()->setSceneData(), 
			//for the m_frame will eventually setSceneData() at Run()
			if(node.valid())
				m_frame.Load(node);

			//add event handler
			FC::GUIEventHandlerArr handlerArr = 
				workFlow->CreateGUIEventHandlerArr();
			for(unsigned int i=0; i<handlerArr.size(); ++i) {
				osgGA::GUIEventHandler* handler = handlerArr[i];
				m_frame.GetViewer()->addEventHandler(handler);
			}
		} else { //����Ĭ�Ϲ�����
			if( !m_frame.Load(std::string(pDoc->m_strOsgFileName)) )
				AfxMessageBox("û��ѡ��osg/ive�ļ����޷����ظ��ļ���");
		}

	}
	m_frame.Run();
}

void COsgFrameWorkView::OnClose()
{
	m_frame.Stop();
	m_frame.ReleaseImpl();

	CView::OnClose();
}

BOOL COsgFrameWorkView::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_ESCAPE) { //��osgViewer����Esc��Ϣ
		return TRUE;
	}

	return CView::PreTranslateMessage(pMsg);
}