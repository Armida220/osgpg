// PointCloudClassifyReader.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "PointCloudClassifyReader.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

/*
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}*/


#ifdef _MANAGED
#pragma managed(pop)
#endif

#include <osg/Geode>
#include <osg/Geometry>
//#include <osgDB/WriteFile>

#include "resource.h"
#include "WaitDialog.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool CreateWorkFlowPlugin(void **pobj)
{
	*pobj = new FC::PointCloudClassifyReader;
	return *pobj != NULL;
}

namespace FC {

	void PointCloudClassifyReader::SetSceneData(osgViewer::CompositeViewer* viewer)
	{
		PointRecArr pointArr;
		if(!ReadPointDataFile(pointArr))
			return;

		osg::Node* geode = (osg::Node*)(PointArr2Geode(pointArr));
		sceneRoot = geode;

		osg::Group* root = viewer->getView(0)->getSceneData()->asGroup();
		root->addChild(geode);
	}

	unsigned int estimatePointsNum(CString fileName)
	{
		//获得文件句柄
		HANDLE hFile=CreateFile(
			fileName,			//文件名
			GENERIC_READ,	//对文件进行读写操作
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,//打开已存在文件
			FILE_ATTRIBUTE_NORMAL,  
			0);

		//返回值size_high,size_low分别表示文件大小的高32位/低32位
		DWORD size_low,size_high;
		size_low= GetFileSize(hFile,&size_high);

		CloseHandle(hFile); //关闭文件

		unsigned int ret = size_low / 30; //经验所得， 返回的是估计点数

		return ret;
	}

	bool PointCloudClassifyReader::ReadPointDataFile(PointRecArr& pointArr)
	{
		pointArr.clear();

		setlocale(LC_ALL, "chs");

		AfxMessageBox("第一步：选择点云分类文件");

		CFileDialog dlg(TRUE, "选择点云分类文件", NULL, 0, "ALL(*.*)|*.*|");
		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择点云分类数据，工作流结束！");
			return false;
		}

		CString strFileName = dlg.GetPathName();

		AfxMessageBox("第二步，读取数据,请耐心等待");

		unsigned int estimateSize = estimatePointsNum(strFileName);

		try {
			pointArr.reserve(estimateSize);
		} catch(...) {
			CString errMsg;
			errMsg.Format("std::vector分配点云分类数组内存出错！点数=%d", estimateSize);
			AfxMessageBox(errMsg);
			return false;
		}

		std::ifstream infile(strFileName);

#ifndef _DEBUG
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CWaitDialog* pWaitDlg = new CWaitDialog;
		pWaitDlg->Create(IDD_DIALOG_WAIT, NULL);
		pWaitDlg->ShowWindow(SW_SHOW);
#endif

#define WAIT_TIME 1000
		int cnt=WAIT_TIME;
		while(infile) {
			PointRec p;
			infile >> p.k >> p.v[0] >> p.v[1] >> p.v[2];
			pointArr.push_back(p);

#ifndef _DEBUG
			--cnt;
			if(cnt==0) {
				pWaitDlg->m_progBar.StepIt();
				cnt=WAIT_TIME;
			}
#endif
		}
#ifndef _DEBUG
		pWaitDlg->DestroyWindow();
		delete pWaitDlg;
#endif

		CString msg;
		msg.Format("读入点数=%u, 进行浏览！", pointArr.size());
		AfxMessageBox(msg);

		return true;
	}

	osg::Geode* PointCloudClassifyReader::PointArr2Geode(PointRecArr& pointArr)
	{
		if(pointArr.size()<=0)
			return 0;

		osg::Geode *geode = new osg::Geode;

		osg::Geometry *galaxy = new osg::Geometry;
		osg::Vec4Array *colorsArr = new osg::Vec4Array;
		osg::Vec3Array *vertexArr = new osg::Vec3Array;
		int nvertices = pointArr.size();

		galaxy->setVertexArray(vertexArr);
		galaxy->setColorArray(colorsArr);

		galaxy->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		for(PointRecArr::iterator pi=pointArr.begin();
			pi!=pointArr.end(); ++pi)
		{
			PointRec& p = (*pi);
			vertexArr->push_back(osg::Vec3(p.v[0], p.v[1], p.v[2]));

			switch(p.k) {
				case WHITE:
					colorsArr->push_back(WHITE_COLOR);
					break;
				case ORANGE:
					colorsArr->push_back(ORANGE_COLOR);
					break;
				case DEEP_GREEN:
					colorsArr->push_back(DEEP_GREEN_COLOR);
					break;
				case LIGHT_GREEN:
					colorsArr->push_back(LIGHT_GREEN_COLOR);
					break;
				case RED:
					colorsArr->push_back(RED_COLOR);
					break;
				default:
					colorsArr->push_back(osg::Vec4(0,0,0,0));
			}
		}

		galaxy->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 
			0, nvertices));
		geode->addDrawable(galaxy);

		return geode;
	}

	void PointCloudClassifyReader::Release()
	{
		delete this;
	}
}