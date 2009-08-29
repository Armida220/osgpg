// ImageMountain.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "ImageMountain.h"
#include "../ImgInterface.h"

#include <osg/ref_ptr>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Image>
#include <osg/PrimitiveSet>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Texture2D>

#include "ImgMountainParaSetter.h"

#define IN_BOUNDARY(x, low, up) ((x)>=(low) && (x)<=(up))
double HEIGHT_SCALE = 20.0;
double PLANE_SCALE = 10.0;

osg::Geode* makeImageMountain(string& imgPath)
{
	ImgFactory imgFactory;
	RgbInterface ri = imgFactory.LoadImage(imgPath.c_str());
	if(!ri.valid()) {	return 0;	}
	GrayInterface gi = imgFactory.ConvertRGB2Gray(ri);

	long H = gi->height/PLANE_SCALE;
	long W = gi->width/PLANE_SCALE;
	long PointNum = (H+2) * (W+2);

	cout<<"H="<<H<<" W="<<W<<endl;
	cout<<"PointNum="<<PointNum<<endl;

	osg::Geode* geode = new osg::Geode;
	osg::Geometry* geom = new osg::Geometry;
	osg::Vec3Array* v3a = new osg::Vec3Array(PointNum);
	geom->setVertexArray(v3a);
	osg::Vec2Array* v2a = new osg::Vec2Array(PointNum);
	geom->setTexCoordArray(0, v2a);

	long cnt=0;
	GLuint* idx = new GLuint[2*W+4];
	for(long y=0; y<=H+1; ++y)
	{
		for(long x=0; x<=W+1; ++x)
		{
			if(IN_BOUNDARY(x, 1, W) && IN_BOUNDARY(y, 1, H)) {
				int rx=(x-1)*PLANE_SCALE;
				int ry=(y-1)*PLANE_SCALE;
				v3a->at(cnt).set(x, y, gi[ry][rx]/HEIGHT_SCALE);
				v2a->at(cnt).set((x-1.0)/W,1-(y-1.0)/H);
			} else {
				v3a->at(cnt).set(x, y, 0);
				v2a->at(cnt).set(0,0);
			}
			
			idx[x*2]	= cnt;
			idx[x*2+1]= cnt+W+2;

			++cnt;
		}

		if(y!=H+1)
			geom->addPrimitiveSet(
				new osg::DrawElementsUInt(
				osg::PrimitiveSet::TRIANGLE_STRIP, 2*W+4, idx)
			);
	}
	delete [] idx;
	
	osg::StateSet* ss = geom->getOrCreateStateSet();
	osg::Image* image = 
		osgDB::readImageFile(imgPath);
	osg::Texture2D* tex = new osg::Texture2D;
	tex->setImage(image);
	ss->setTextureAttributeAndModes(0, tex);

	geode->addDrawable(geom);

	return geode;
}

//////////////////////////////////////////////////////////////////////////

bool CreateWorkFlowPlugin(void **pobj)
{
	*pobj = new FC::ImageMountain;
	return *pobj != NULL;
}

namespace FC {
	void ImageMountain::Release()
	{
		delete this;
	}
	void ImageMountain::SetSceneData(osgViewer::CompositeViewer* viewer)
	{
		osg::Group* root = viewer->getView(0)->getSceneData()->asGroup();

		setlocale(LC_ALL, "chs");

		AfxMessageBox("第一步：选择图像文件");

		CFileDialog dlg(TRUE, "选择图像文件", NULL, 0, "ALL(*.*)|*.*|");
		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择图像文件，工作流结束！");
			return;
		}
		CString strFileName = dlg.GetPathName();

		string imgPath(strFileName);

		AfxMessageBox("第二步：设置参数");
#ifndef _DEBUG
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		ImgMountainParaSetter ps;
		if(ps.DoModal()!=IDOK) {
			AfxMessageBox("未设置参数，退出工作流！");
			return;
		}

		HEIGHT_SCALE = ps.m_heightScale;
		PLANE_SCALE = ps.m_planescale;
#endif

		this->sceneRoot = makeImageMountain(imgPath);

		root->addChild(sceneRoot);

		AfxMessageBox("第三步：浏览ImageMountain模型");
	}
}