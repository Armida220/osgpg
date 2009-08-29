// FishEyeImageVR.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "FishEyeImageVR.h"
#include "../ImgInterface.h"
#include "FishEyeImageAPI.h"

#include <osg/ref_ptr>
#include <osg/Geometry>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Image>
#include <osg/PrimitiveSet>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Texture2D>
using namespace std;

osg::Geode* makeFishEyeSemiBall(string& imgPath, int flag=1)
{
	ImgFactory imgFactory;
	RgbInterface rawPic = imgFactory.LoadImage(imgPath.c_str());
	if(!rawPic.valid()) {	return 0;	}

	//determine x0,y0,r
	double x0,y0,r;
	GetFishEyePhotoParas(rawPic, x0, y0, r, false);
	//r-=20;

	double H = rawPic->height;
	double W = rawPic->width;

	// create a container that makes the sphere drawable
	osg::Geometry *sPlanetSphere = new osg::Geometry();

	{
		// now set up the coords, normals and texcoords for geometry 
		unsigned int numX = 100;
		unsigned int numY = 50;
		unsigned int numVertices = numX*numY;

		osg::Vec3Array* coords = new osg::Vec3Array(numVertices);
		sPlanetSphere->setVertexArray(coords);

		osg::Vec3Array* normals = new osg::Vec3Array(numVertices);
		sPlanetSphere->setNormalArray(normals);
		sPlanetSphere->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

		osg::Vec2Array* texcoords = new osg::Vec2Array(numVertices);
		sPlanetSphere->setTexCoordArray(0,texcoords);

		double delta_elevation = osg::PI / (double)(numY-1);
		//double delta_azim = 2.0*osg::PI / (double)(numX-1);
		double delta_azim = flag * osg::PI / (double)(numX-1);

		double elevation = -osg::PI*0.5;
		unsigned int vert = 0;
		unsigned j;
		for(j=0;
			j<numY;
			++j, elevation+=delta_elevation )
		{
			double azim = 0.0;
			for(unsigned int i=0;
				i<numX;
				++i, ++vert, azim+=delta_azim)
			{
				osg::Vec3 direction(cos(azim)*cos(elevation), 
					sin(azim)*cos(elevation), sin(elevation));
				(*normals)[vert].set(-direction);

				osg::Vec3 xyz = direction*r;
				(*coords)[vert].set(xyz);

				xyz.x() = flag * xyz.x();
				(*texcoords)[vert].set((xyz.x()+x0)/W, (xyz.z()+y0)/H);
			}
		}

		for(j=0;
			j<numY-1;
			++j)
		{
			unsigned int curr_row = j*numX;
			unsigned int next_row = curr_row+numX;
			osg::DrawElementsUShort* elements = 
				new osg::DrawElementsUShort(GL_QUAD_STRIP);
			for(unsigned int i=0;
				i<numX;
				++i)
			{
				elements->push_back(next_row + i);
				elements->push_back(curr_row + i);
			}
			sPlanetSphere->addPrimitiveSet(elements);
		}
	}

	// create a geode object to as a container for our drawable sphere object
	osg::Geode* geodePlanet = new osg::Geode();

	{
		osg::Image* image = osgDB::readImageFile( imgPath );
		if ( image )
		{
			osg::Texture2D* tex2d = new osg::Texture2D( image );
			tex2d->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
			tex2d->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );
			geodePlanet->getOrCreateStateSet()->setTextureAttributeAndModes( 
				0, tex2d, osg::StateAttribute::ON );
		}
	}

	// add our drawable sphere to the geode container
	geodePlanet->addDrawable( sPlanetSphere );

	return( geodePlanet );
}

//////////////////////////////////////////////////////////////////////////
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/GUIEventHandler>

class LensController : public osgGA::GUIEventHandler
{
	osg::Camera* _cam;
	double t,b,l,r,n,f;
public:
	double depth;
	LensController() {}
	void SetCameraToControl(osg::Camera* cam) { _cam = cam; }
	
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
	{
		if(_cam==0)
			return false;
		switch(ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYDOWN):
			_cam->getProjectionMatrixAsFrustum(l, r, b, t, n, f);
			static double st=t,sb=b,sl=l,sr=r,sn=n,sf=f;
			if(ea.getKey()=='n')
			{
				n+=0.1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()=='m')
			{
				n-=0.1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()=='f')
			{
				f+=0.1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()=='g')
			{
				f-=0.1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Up)
			{
				t+=0.1;b+=0.1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Down)
			{
				t-=0.1;b-=0.1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Left)
			{
				l-=0.1;r-=0.1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Right)
			{
				l+=0.1;r+=0.1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Page_Up
				||ea.getKey()==osgGA::GUIEventAdapter::KEY_Page_Down)
			{
				_cam->setProjectionMatrixAsFrustum(sl,sr,sb,st,sn,sf);
			}
			return true;
		}
		return false;
	}
};

class TrackballAllRotateNoMove : 
	public osgGA::TrackballManipulator
{
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& us)
	{
		if(ea.getButtonMask()!=
			osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
				return TrackballManipulator::handle(ea,us);
		}
		return true;
	}

	virtual void computeHomePosition()
	{
		if(getNode())
		{
			const osg::BoundingSphere& boundingSphere=getNode()->getBound();

			setHomePosition(boundingSphere._center,
				boundingSphere._center/*+osg::Vec3(0.0f,1.0f,0.0f)*boundingSphere.radius()*/,
				osg::Vec3(0.0f,0.0f,1.0f),
				true);
		}
	}
};
//////////////////////////////////////////////////////////////////////////
bool CreateWorkFlowPlugin(void **pobj)
{
	*pobj = new FC::FishEyeImageVR;
	return *pobj != NULL;
}

namespace FC {
	void FishEyeImageVR::Release()
	{
		delete this;
	}
	void FishEyeImageVR::SetSceneData(osgViewer::CompositeViewer* viewer)
	{
		osg::Group* root = viewer->getView(0)->getSceneData()->asGroup();

		setlocale(LC_ALL, "chs");

		AfxMessageBox("第一步：选择第一幅图像文件");

		CFileDialog dlg(TRUE, "选择图像文件", NULL, 0, "ALL(*.*)|*.*|");
		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择图像文件，工作流结束！");
			return;
		}
		CString strFileName = dlg.GetPathName();

		string imgPath(strFileName);

		AfxMessageBox("第二步：选择第二幅图像文件");

		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择图像文件，工作流结束！");
			return;
		}
		strFileName = dlg.GetPathName();

		string img2Path(strFileName);

		//AfxMessageBox("第三步：设置参数");
#ifndef _DEBUG
// 		AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 		ImgMountainParaSetter ps;
// 		if(ps.DoModal()!=IDOK) {
// 			AfxMessageBox("未设置参数，退出工作流！");
// 			return;
// 		}
#endif

		osg::Group* gp = new osg::Group;
		this->sceneRoot = gp;
		gp->addChild( makeFishEyeSemiBall(imgPath) );
		gp->addChild( makeFishEyeSemiBall(img2Path, -1) );

		root->addChild(sceneRoot);

		viewer->getView(0)->setCameraManipulator(new TrackballAllRotateNoMove);
		LensController* lc = new LensController;
		lc->SetCameraToControl(viewer->getView(0)->getCamera());
		viewer->getView(0)->addEventHandler(lc);

		AfxMessageBox("第三步：浏览FishEyeVR模型");
	}
}