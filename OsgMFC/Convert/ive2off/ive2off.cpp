// ive2off.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "ive2off.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osg/NodeVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/NodeVisitor>
#include <osg/Drawable>
#include <osg/TriangleFunctor>
#include <osg/TriangleIndexFunctor>
#include <osg/MatrixTransform>
#include <osg/Matrix>
#include <osg/Node>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

typedef unsigned int Uint;

struct PointIdx {
	PointIdx(Uint i1, Uint i2, Uint i3) { i[0] = i1; i[1] = i2; i[2] = i3; }
	Uint i[3];
};
typedef vector<PointIdx> PointIdxArr;

struct TriangleIndexPrinter
{
	Uint _offset;
	PointIdxArr* pia;
	void operator() (const Uint& i1,const Uint& i2,const Uint& i3) const 
	{
		pia->push_back(PointIdx(i1+_offset, i2+_offset, i3+_offset));
	}
};

class PointVisitor : public osg::NodeVisitor
{
protected:
	osg::TriangleIndexFunctor<TriangleIndexPrinter> tif;
	PointIdxArr pia;
	osg::ref_ptr<osg::Vec3Array> vert;
public:
	PointVisitor()
	{
		setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
		vert = new osg::Vec3Array;
	}

	PointIdxArr& GetPointIdxArray() { return pia; }
	osg::Vec3Array& GetVertexArray() { return *vert; }

	void apply(osg::Geode& node)
	{
		osg::NodePath nodePathToRoot = this->getNodePath();
		osg::Matrix localToWorld = osg::computeLocalToWorld(nodePathToRoot);

		for(Uint i=0; i<node.getNumDrawables(); ++i) {
			osg::Drawable* drawable = node.getDrawable(i);
			osg::Geometry* geom = dynamic_cast<osg::Geometry*>(drawable);
			if(!geom)
				continue;
			osg::Vec3Array* local_vert = 
				dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			if(!local_vert)
				continue;

			//push index
			tif._offset = vert->size();
			tif.pia = &pia;
			geom->accept(tif);

			//push vertex
			for(osg::Vec3Array::iterator itr = local_vert->begin();
				itr!=local_vert->end(); ++itr) {
					osg::Vec3& p = *itr;

					vert->push_back(p*localToWorld);
			}
		}
		traverse(node);
	}
};

std::ofstream& operator<<(std::ofstream& out, osg::Vec3& p)
{
	out<<p.x()<<" "<<p.y()<<" "<<p.z();
	return out;
}

std::ofstream& operator<<(std::ofstream& out, PointIdx& pi)
{
	out<<"3 "<<pi.i[0]<<" "<<pi.i[1]<<" "<<pi.i[2];
	return out;
}


void OutPutAsOFF(osg::Node* root, string saveName)
{
	PointVisitor pv;
	root->accept(pv);

	PointIdxArr& pia = pv.GetPointIdxArray();
	osg::Vec3Array& vert = pv.GetVertexArray();

	std::ofstream out(saveName.c_str());

	out<<"OFF"<<endl;
	out<<vert.size()<<" "<<pia.size()<<" 0"<<endl;

	for(Uint i=0; i<vert.size(); ++i) {
		osg::Vec3& p = vert[i];
		out<<p<<endl;
	}

	for(Uint i=0; i<pia.size(); ++i) {
		PointIdx& pi = pia[i];
		out<<pi<<endl;
	}

	out.close();
}

//////////////////////////////////////////////////////////////////////////

bool CreateWorkFlowPlugin(void **pobj)
{
	*pobj = new FC::Ive2off;
	return *pobj != NULL;
}

namespace FC {
	void Ive2off::Release()
	{
		delete this;
	}

	osg::Node* Ive2off::CreateSceneData()
	{
		setlocale(LC_ALL, "chs");

		AfxMessageBox("第一步：选择待转换osg或ive文件");

		CFileDialog dlg(TRUE, "选择待转换osg或ive文件", NULL, 0, "osg(*.osg)|*.osg|ive(*.ive)|*.ive|");
		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择osg或ive文件，工作流结束！");
			return 0;
		}
		CString strFileName = dlg.GetPathName();

		AfxMessageBox("第二步：读取该文件");
		osg::Node* ret = osgDB::readNodeFile(string(strFileName));
		sceneRoot = ret;

		if(ret==0) {
			AfxMessageBox("无法读取该文件！");
			return 0;
		}

		AfxMessageBox("第三步：转换成off文件并保存");
		CFileDialog saveDlg(FALSE, "选择保存文件名", NULL, 0, "off文件(*.off)|*.off|All(*.*)|*.*|");
		if(saveDlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择保存文件名！");
			return ret;
		}
		CString strSaveFileName = saveDlg.GetPathName();
		OutPutAsOFF(ret, string(strSaveFileName));

		return ret;
	}
}