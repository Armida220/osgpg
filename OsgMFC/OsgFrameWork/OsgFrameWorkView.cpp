// OsgFrameWorkView.cpp : COsgFrameWorkView ���ʵ��
//

#include "stdafx.h"
#include "OsgFrameWork.h"

#include "OsgFrameWorkDoc.h"
#include "OsgFrameWorkView.h"

#include "MainFrm.h"

#include "PluginManager.h"
#include <osgGA/GUIEventHandler>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COsgFrameWorkView

IMPLEMENT_DYNCREATE(COsgFrameWorkView, CView)

BEGIN_MESSAGE_MAP(COsgFrameWorkView, CView)
	ON_WM_CLOSE()
	ON_COMMAND(ID_ADD_CONTROLLER_PLUGIN, &COsgFrameWorkView::OnAddControllerPlugin)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// COsgFrameWorkView ����/����

COsgFrameWorkView::COsgFrameWorkView()
{
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

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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

// COsgFrameWorkView ��Ϣ�������

void COsgFrameWorkView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CRect rect;
	GetClientRect(&rect);

	COsgFrameWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc) {
		FC::PluginManager& pm = 
			pDoc->m_pluginManager;

		FC::WorkFlowPluginBase* workFlow =
			pm.GetWorkFlow();

		if(workFlow) { //���ȳ��� ���������
			//1. try new impl of ViewerFramework
			m_frame.ResetImpl(
				workFlow->CreateViewerFrameworkImpl() );
			m_frame.SetupWindow(GetSafeHwnd(), rect);

			//2. add scene data
			m_frame.InitSceneRoot();
			workFlow->SetSceneData(m_frame.GetViewer());

			//3. add event handlers
			workFlow->SetGUIEventHandlers(m_frame.GetViewer());
		} else { //����Ĭ�Ϲ�����
			m_frame.SetupWindow(GetSafeHwnd(), rect);
			if( !m_frame.AddModel(std::string(pDoc->m_strOsgFileName)) )
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

void COsgFrameWorkView::OnAddControllerPlugin()
{
	CFileDialog dlg(TRUE, "�򿪿��������", NULL, 0, "plugin���(*.plugin)|*.plugin|�����ļ�(*.*)|*.*||");
	if(dlg.DoModal()!=IDOK) {
		AfxMessageBox("û��ѡ���κβ����");
		return;
	}
	CString name = dlg.GetPathName();

	COsgFrameWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	FC::PluginManager& pm = pDoc->m_pluginManager;

	FC::ControllerPluginBase* ctrlCreator = 0;
	if( ctrlCreator = pm.AddControllerPlugin(name) )
	{
		AfxMessageBox("�ɹ����ؿ����������");
	} else {
		AfxMessageBox("����ʧ�ܣ�");
		return;
	}

	int ViewID=0; // TODO Ĭ���������0��View

	osgGA::GUIEventHandler* tmp = ctrlCreator->CreateController();
	osgGA::MatrixManipulator* ctrl = 
		dynamic_cast<osgGA::MatrixManipulator*>(tmp);
	if(ctrl)
		m_frame.GetViewer()->getView(ViewID)->setCameraManipulator(ctrl);
}

void COsgFrameWorkView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);

	m_frame.Resize(rect);
}