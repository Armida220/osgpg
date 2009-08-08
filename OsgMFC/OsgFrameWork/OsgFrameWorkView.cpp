// OsgFrameWorkView.cpp : COsgFrameWorkView 类的实现
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

// COsgFrameWorkView 构造/析构

COsgFrameWorkView::COsgFrameWorkView()
{
}

COsgFrameWorkView::~COsgFrameWorkView()
{
}

BOOL COsgFrameWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COsgFrameWorkView 绘制

void COsgFrameWorkView::OnDraw(CDC* /*pDC*/)
{
	COsgFrameWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// COsgFrameWorkView 诊断

#ifdef _DEBUG
void COsgFrameWorkView::AssertValid() const
{
	CView::AssertValid();
}

void COsgFrameWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COsgFrameWorkDoc* COsgFrameWorkView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COsgFrameWorkDoc)));
	return (COsgFrameWorkDoc*)m_pDocument;
}
#endif //_DEBUG

// COsgFrameWorkView 消息处理程序

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

		if(workFlow) { //首先尝试 插件工作流
			//1. try new impl of ViewerFramework
			m_frame.ResetImpl(
				workFlow->CreateViewerFrameworkImpl() );
			m_frame.SetupWindow(GetSafeHwnd(), rect);

			//2. add scene data
			m_frame.InitSceneRoot();
			workFlow->SetSceneData(m_frame.GetViewer());

			//3. add event handlers
			workFlow->SetGUIEventHandlers(m_frame.GetViewer());
		} else { //进行默认工作流
			m_frame.SetupWindow(GetSafeHwnd(), rect);
			if( !m_frame.AddModel(std::string(pDoc->m_strOsgFileName)) )
				AfxMessageBox("没有选择osg/ive文件或无法加载该文件！");
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
	if(pMsg->wParam == VK_ESCAPE) { //对osgViewer屏蔽Esc消息
		return TRUE;
	}

	return CView::PreTranslateMessage(pMsg);
}

void COsgFrameWorkView::OnAddControllerPlugin()
{
	CFileDialog dlg(TRUE, "打开控制器插件", NULL, 0, "plugin插件(*.plugin)|*.plugin|其他文件(*.*)|*.*||");
	if(dlg.DoModal()!=IDOK) {
		AfxMessageBox("没有选择任何插件！");
		return;
	}
	CString name = dlg.GetPathName();

	COsgFrameWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	FC::PluginManager& pm = pDoc->m_pluginManager;

	FC::ControllerPluginBase* ctrlCreator = 0;
	if( ctrlCreator = pm.AddControllerPlugin(name) )
	{
		AfxMessageBox("成功加载控制器插件！");
	} else {
		AfxMessageBox("加载失败！");
		return;
	}

	int ViewID=0; // TODO 默认添加至第0号View

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