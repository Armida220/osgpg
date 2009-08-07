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
END_MESSAGE_MAP()

// COsgFrameWorkView 构造/析构

COsgFrameWorkView::COsgFrameWorkView()
{
	// TODO: 在此处添加构造代码

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

	m_frame.SetupWindow(GetSafeHwnd(), rect);

	//get main frame
	COsgFrameWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc) {
		FC::PluginManager& pm = 
			pDoc->m_pluginManager;

		FC::WorkFlowPluginBase* workFlow =
			pm.GetWorkFlow();

		if(workFlow) { //首先尝试 插件工作流
			//add scene data
			osg::ref_ptr<osg::Node> node = 
				workFlow->CreateSceneData();
			//ATTENTION! can not use m_frame.GetViewer()->setSceneData(), 
			//for the m_frame will eventually setSceneData() at Run()
			if(node.valid())
				m_frame.Load(node);

			//add event handlers
			FC::GUIEventHandlerArr handlerArr = 
				workFlow->CreateGUIEventHandlerArr();
			for(unsigned int i=0; i<handlerArr.size(); ++i) {
				osgGA::GUIEventHandler* handler = handlerArr[i];
				m_frame.GetViewer()->addEventHandler(handler);
			}
		} else { //进行默认工作流
			if( !m_frame.Load(std::string(pDoc->m_strOsgFileName)) )
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

	osgGA::GUIEventHandler* tmp = ctrlCreator->CreateController();
	osgGA::MatrixManipulator* ctrl = 
		dynamic_cast<osgGA::MatrixManipulator*>(tmp);
	if(ctrl)
		m_frame.GetViewer()->setCameraManipulator(ctrl);
}
