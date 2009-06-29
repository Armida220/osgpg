// OsgMFCView.cpp : COsgMFCView 类的实现
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

// COsgMFCView 构造/析构

COsgMFCView::COsgMFCView()
{
	m_Closed_yet=true;
}

COsgMFCView::~COsgMFCView()
{
}

BOOL COsgMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COsgMFCView 绘制

void COsgMFCView::OnDraw(CDC* /*pDC*/)
{
	COsgMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// COsgMFCView 诊断

#ifdef _DEBUG
void COsgMFCView::AssertValid() const
{
	CView::AssertValid();
}

void COsgMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COsgMFCDoc* COsgMFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COsgMFCDoc)));
	return (COsgMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// COsgMFCView 消息处理程序

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

	// TODO: 在此处添加消息处理程序代码
}

void COsgMFCView::OnOsgRun()
{
	if(!m_Closed_yet) {
		AfxMessageBox("请先停止当前工作流，再重新开始！");
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
	CFileDialog dlg(TRUE, NULL, NULL, 0, "DLL文件(*.dll)|*.dll");
	if(dlg.DoModal()!=IDOK) {
		return;
	}
	CString strDllFileName = dlg.GetPathName();

	if( m_pluginManager.AddPlugin(strDllFileName) )
	{
		AfxMessageBox("成功加载插件！");
	}
}

void COsgMFCView::OnOsgStop()
{
	m_frame.Stop();
	m_Closed_yet = true;
	Invalidate(true);
}
