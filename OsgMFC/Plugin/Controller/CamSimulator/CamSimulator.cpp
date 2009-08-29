
#include "stdafx.h"

#include "CamSimulator.h"

#include <osg/ref_ptr>
#include <osg/Group>
#include <osg/NodeVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Geometry>
#include <osg/DisplaySettings>
#include <osg/MatrixTransform>
#include <osg/LineWidth>
#include <osgDB/ReadFile>
#include <osgViewer/CompositeViewer>
#include <osg/Texture2D>
#include <osg/Matrix>
#include <osg/CullFace>
#include <osg/Quat>
#include <osg/NodeCallback>
#include <osg/Drawable>
#include <osg/Viewport>
#include <osgGA/TrackballManipulator>
#include "osgGA/GUIEventHandler"
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include "CamCalibrator.h"
#include "CamParameters.h"
#include "PixelGetter.h"
#include <iostream>
using namespace std;

struct CamTriple {
	CamTriple(const CamTriple& rhs) : cp(rhs.cp)
		, photoNode(rhs.photoNode), frustumNode(rhs.frustumNode)
  {}
	CamTriple() : photoNode(0), frustumNode(0) {}
	CamTriple(std::ifstream& camFile) : cp(camFile)
		, photoNode(0), frustumNode(0)	{}
	FC::CamParameters cp;
	osg::ref_ptr<osg::Node> photoNode;
	osg::ref_ptr<osg::Node> frustumNode;
};
typedef list<CamTriple> CamList;
typedef CamList::iterator CamItr;

//camsFile format:
//numOfCam
//CamParameters 1
//...
void ReadCamList(CamList& cl, std::ifstream& camsFile)
{
	cl.clear();
	FC::Uint num;
	camsFile >> num;
	for(FC::Uint i=0; i<num; ++i) {
		CamTriple ct(camsFile);
		cl.push_back(ct);
	}
}

void ResetCamera(osg::Camera* cam, double depth, FC::CamParameters& cp);

class ColorResetter : public osg::NodeVisitor
{
public:
	FC::CamParameters& _cp;
	ColorResetter(FC::CamParameters& cp) : _cp(cp) {
		setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
		_cp.pg.Load(_cp.photoPath.c_str());
	}

	virtual void apply(osg::Geode& node)
	{
		for(unsigned int j=0; j<node.getNumDrawables(); ++j) {
			osg::Geometry* geom = node.getDrawable(j)->asGeometry();
			if(geom) {
				osg::Vec3Array* v3a = 
					dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
				if(v3a) {
					geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
					osg::ref_ptr<osg::Vec4Array> v4a = new osg::Vec4Array(v3a->size());
					//	dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
					for(unsigned int i=0; i<v3a->size(); ++i) {
						osg::Vec3& v3 = v3a->at(i);
						double u,v;
						_cp.project(v3.x(), v3.y(), v3.z(), u, v);
						osg::Vec4& v4 = v4a->at(i);
						_cp.pg.GetPixel(u,v,v4);
					}
					v4a->dirty();
					geom->setColorArray(v4a);
				}
			}
		}
	}
};

class LensController : public osgGA::GUIEventHandler
{
	CamList _cl;
	CamItr _curC;

	osg::Camera* _cam;
	double t,b,l,r,n,f;

	osg::Group* lensRoot;

	void NextCam() { _curC++; if(_curC==_cl.end()) _curC = FirstCam(); }
	void PreCam() { if(_curC==_cl.begin()) _curC=_cl.end(); _curC--; }

public:
	osg::ref_ptr<osg::Node> scenePoints;
	double depth;
	LensController() {}
	void SetCameraToControl(osg::Camera* cam) { _cam = cam; }
	void SetLensRoot(osg::Group* root) { lensRoot = root; }
	CamList& GetCamList() { return _cl; }
	CamItr FirstCam() { return _curC = _cl.begin(); }

	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
	{
		if(_cam==0)
			return false;
		switch(ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYDOWN):
			_cam->getProjectionMatrixAsFrustum(l, r, b, t, n, f);
			if(ea.getKey()=='n')
			{
				n+=1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()=='m')
			{
				n-=1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()=='f')
			{
				f+=1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()=='g')
			{
				f-=1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Up)
			{
				t+=1;b+=1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Down)
			{
				t-=1;b-=1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Left)
			{
				l-=1;r-=1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Right)
			{
				l+=1;r+=1;
				_cam->setProjectionMatrixAsFrustum(l,r,b,t,n,f);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Page_Up)
			{
				PreCam();
				ResetCamera(_cam, depth, _curC->cp);
				lensRoot->replaceChild(lensRoot->getChild(1),
					_curC->photoNode);
			}
			if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Page_Down)
			{
				NextCam();
				ResetCamera(_cam, depth, _curC->cp);
				lensRoot->replaceChild(lensRoot->getChild(1),
					_curC->photoNode);
			}
			if(ea.getKey()=='p') {
				ColorResetter cr(_curC->cp);
				scenePoints->accept(cr);
			}
			return true;
		}
		return false;
	}
};

class CameraUpdateCallBack : public osg::Drawable::UpdateCallback 
{
public:
	osg::Camera* camera;
	virtual void update(osg::NodeVisitor* nv, osg::Drawable* drawable)
	{
		osg::Matrixd proj;
		if (camera)
		{
			proj = camera->getProjectionMatrix();
		}

		const double znear = proj(3,2) / (proj(2,2)-1.0);
		const double zfar = proj(3,2) / (1.0+proj(2,2));

		const double nLeft = znear * (proj(2,0)-1.0) / proj(0,0);
		const double nRight = znear * (1.0+proj(2,0)) / proj(0,0);
		const double nTop = znear * (1.0+proj(2,1)) / proj(1,1);
		const double nBottom = znear * (proj(2,1)-1.0) / proj(1,1);

		const double fLeft = zfar * (proj(2,0)-1.0) / proj(0,0);
		const double fRight = zfar * (1.0+proj(2,0)) / proj(0,0);
		const double fTop = zfar * (1.0+proj(2,1)) / proj(1,1);
		const double fBottom = zfar * (proj(2,1)-1.0) / proj(1,1);

		osg::Geometry* geom = drawable->asGeometry();
		osg::Vec3Array* v = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
		(*v)[0].set( 0., 0., 0. );
		(*v)[1].set( nLeft, nBottom, -znear );
		(*v)[2].set( nRight, nBottom, -znear );
		(*v)[3].set( nRight, nTop, -znear );
		(*v)[4].set( nLeft, nTop, -znear );
		(*v)[5].set( fLeft, fBottom, -zfar );
		(*v)[6].set( fRight, fBottom, -zfar );
		(*v)[7].set( fRight, fTop, -zfar );
		(*v)[8].set( fLeft, fTop, -zfar );
		v->dirty();
	}
};

osg::Node*
makeImageFromCamera( osg::Camera* camera, osg::Image* image)
{
	osg::Matrixd proj;
	osg::Matrixd mv;
	if (camera)
	{
		proj = camera->getProjectionMatrix();
		mv = camera->getViewMatrix();
	}

	const double znear = proj(3,2) / (proj(2,2)-1.0);
	const double zfar = proj(3,2) / (1.0+proj(2,2)) - 1;

	const double fLeft = zfar * (proj(2,0)-1.0) / proj(0,0);
	const double fRight = zfar * (1.0+proj(2,0)) / proj(0,0);
	const double fTop = zfar * (1.0+proj(2,1)) / proj(1,1);
	const double fBottom = zfar * (proj(2,1)-1.0) / proj(1,1);

	osg::Vec3Array* v = new osg::Vec3Array;
	v->resize( 4 );
	(*v)[0].set( fLeft, fBottom, -zfar );
	(*v)[1].set( fRight, fBottom, -zfar );
	(*v)[2].set( fRight, fTop, -zfar );
	(*v)[3].set( fLeft, fTop, -zfar );

	osg::Vec2Array* vt = new osg::Vec2Array;
	vt->resize( 4 );
	(*vt)[0].set( 0,0 );
	(*vt)[1].set( 1,0 );
	(*vt)[2].set( 1,1 );
	(*vt)[3].set( 0,1 );

	osg::Geometry* geom = new osg::Geometry;
	geom->setUseDisplayList( false );
	geom->setVertexArray( v );
	geom->setTexCoordArray(0, vt);

	osg::Vec4Array* c = new osg::Vec4Array;
	c->push_back( osg::Vec4( 1., 1., 1., 1. ) );
	geom->setColorArray( c );
	geom->setColorBinding( osg::Geometry::BIND_OVERALL );

	GLushort idxLoops0[4] = {	0, 1, 2, 3 };
	geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::QUADS, 4, idxLoops0 ) );

	osg::StateSet* stateset = geom->getOrCreateStateSet();
	osg::Texture2D* texture = new osg::Texture2D;
	texture->setImage(image);
	stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

	//ImageUpdateCallBack* imgcb = new ImageUpdateCallBack;
	//imgcb->camera = camera;
	//geom->setUpdateCallback( imgcb );

	osg::Geode* geode = new osg::Geode;
	geode->addDrawable( geom );

	geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
	geode->getOrCreateStateSet()->setMode( GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );

	osg::MatrixTransform* mt = new osg::MatrixTransform;
	mt->setMatrix( osg::Matrixd::inverse( mv ) );
	mt->addChild( geode );

	return mt;
}

osg::Node*
makeFrustumFromCamera( osg::Camera* camera )
{
	osg::Matrixd proj;
	osg::Matrixd mv;
	if (camera)
	{
		proj = camera->getProjectionMatrix();
		mv = camera->getViewMatrix();
	}

	const double znear = proj(3,2) / (proj(2,2)-1.0);
	const double zfar = proj(3,2) / (1.0+proj(2,2));

	const double nLeft = znear * (proj(2,0)-1.0) / proj(0,0);
	const double nRight = znear * (1.0+proj(2,0)) / proj(0,0);
	const double nTop = znear * (1.0+proj(2,1)) / proj(1,1);
	const double nBottom = znear * (proj(2,1)-1.0) / proj(1,1);

	const double fLeft = zfar * (proj(2,0)-1.0) / proj(0,0);
	const double fRight = zfar * (1.0+proj(2,0)) / proj(0,0);
	const double fTop = zfar * (1.0+proj(2,1)) / proj(1,1);
	const double fBottom = zfar * (proj(2,1)-1.0) / proj(1,1);

	osg::Vec3Array* v = new osg::Vec3Array;
	v->resize( 9 );
	(*v)[0].set( 0., 0., 0. );
	(*v)[1].set( nLeft, nBottom, -znear );
	(*v)[2].set( nRight, nBottom, -znear );
	(*v)[3].set( nRight, nTop, -znear );
	(*v)[4].set( nLeft, nTop, -znear );
	(*v)[5].set( fLeft, fBottom, -zfar );
	(*v)[6].set( fRight, fBottom, -zfar );
	(*v)[7].set( fRight, fTop, -zfar );
	(*v)[8].set( fLeft, fTop, -zfar );

	osg::Geometry* geom = new osg::Geometry;
	geom->setUseDisplayList( false );
	geom->setVertexArray( v );

	osg::Vec4Array* c = new osg::Vec4Array;
	c->push_back( osg::Vec4( 1., 1., 1., 1. ) );
	geom->setColorArray( c );
	geom->setColorBinding( osg::Geometry::BIND_OVERALL );

	GLushort idxLines[8] = {0, 5, 0, 6, 0, 7, 0, 8 };
	GLushort idxLoops0[4] = {	1, 2, 3, 4 };
	GLushort idxLoops1[4] = {	5, 6, 7, 8 };
	geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINES, 8, idxLines ) );
	geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops0 ) );
	geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops1 ) );

	CameraUpdateCallBack* cucb = new CameraUpdateCallBack;
	cucb->camera = camera;
	geom->setUpdateCallback( cucb );

	osg::Geode* geode = new osg::Geode;
	geode->addDrawable( geom );

	geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );

	osg::MatrixTransform* mt = new osg::MatrixTransform;
	mt->setMatrix( osg::Matrixd::inverse( mv ) );
	mt->addChild( geode );

	return mt;
}

void ResetCamera(osg::Camera* cam, double depth, FC::CamParameters& cp)
{
	cp.computeParameters();
	osg::Vec3 eye, look, up;
	cp.computeLookAt(eye, look, up);
	cam->setViewMatrixAsLookAt(eye, eye + look, up);
	double frustum[6];
	cp.computerDefaultFrustum(depth, frustum);
	cam->setProjectionMatrixAsFrustum(
			frustum[Left], frustum[Right], 
			frustum[Bottom], frustum[Top], 
			frustum[Near], frustum[Far]);
}


//////////////////////////////////////////////////////////////////////////

bool CreateWorkFlowPlugin(void **pobj)
{
	*pobj = new FC::CamSimulator;
	return *pobj != NULL;
}

namespace FC {
	void CamSimulator::Release()
	{
		delete this;
	}

	void CamSimulator::SetSceneData(osgViewer::CompositeViewer* viewer)
	{
		setlocale(LC_ALL, "chs");
		osg::ref_ptr< osg::Group > root0 = 
			viewer->getView(0)->getSceneData()->asGroup();

		//1. add axes
		//osg::Node* axes = osgDB::readNodeFile(string("axes.osg"));
		//root0->addChild( axes );
		root0->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );

		//2. add point cloud
		AfxMessageBox("第一步：选择点模型文件");

		CFileDialog dlg(TRUE, "选择点模型（osg或ive文件）", NULL, 0, "osg(*.osg)|*.osg|ive(*.ive)|*.ive|");
		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择osg或ive文件，工作流结束！");
			return;
		}
		CString strFileName = dlg.GetPathName();

		string pointPath(strFileName);
		osg::ref_ptr< osg::Node > scenePoints;
		scenePoints = osgDB::readNodeFile( pointPath );
		if (!scenePoints.valid())	{
			AfxMessageBox("无法读入点模型，工作流结束！");
			return;
		}
		scenePoints->setName("PointCloud");

		//3. set scenedata for view 0
		//{
			//scenePoints will be in root1
			//root0->addChild(scenePoints);
		//}

		//4. set scenedata for view 1
		{
			osgViewer::View* view = viewer->getView(1);
			LensController* lensCtrl = new LensController;
			lensCtrl->SetCameraToControl(view->getCamera());
			////////////////////////////////////////////////////////////////////////
			AfxMessageBox("第二步：选择相机文件");

			CFileDialog dlg(TRUE, "选择相机文件", NULL, 0, "All(*.*)|*.*|");
			if(dlg.DoModal()!=IDOK) {
				AfxMessageBox("没有选择文件，工作流结束！");
				return;
			}
			CString strFileName = dlg.GetPathName();

			string camFilePath(strFileName);
			std::ifstream camFile(camFilePath.c_str());
			CamList& cl = lensCtrl->GetCamList();
			ReadCamList(cl, camFile);

			CamItr cItr;
			FC::Uint i=0;
			osg::Camera* cam = view->getCamera();
			lensCtrl->depth = scenePoints->getBound().radius()*2+1;
			for(cItr = cl.begin(); cItr!=cl.end(); ++cItr, ++i) {
				//step 1 calibration
				ResetCamera(cam, lensCtrl->depth, cItr->cp);

				//step 2 photo
				osg::ref_ptr<osg::Image> image;
				image = osgDB::readImageFile(cItr->cp.photoPath);
				cItr->photoNode = makeImageFromCamera(cam, image);
				char name[1024];
				sprintf(name, "Photo%i", i);
				cItr->photoNode->setName(name);
				root0->addChild(cItr->photoNode);

				//step 3 frustum
				cItr->frustumNode = makeFrustumFromCamera( cam );
				char name1[1024];
				sprintf(name1, "Frustum%i", i);
				cItr->frustumNode->setName(name1);
				root0->addChild( cItr->frustumNode );
			}
			//////////////////////////////////////////////////////////////////////////
			osg::Group* root1 = view->getSceneData()->asGroup();
			lensCtrl->scenePoints = scenePoints;
			lensCtrl->SetLensRoot(root1);
			root1->addChild(scenePoints);			//child 0
			cItr = lensCtrl->FirstCam();
			root1->addChild(cItr->photoNode);	//child 1
			//now scene look like this:
			//              root0
			//             /  \  \
			//         root1   \    \
			//           |  \   \      \
			// scenePoints  photos    frustums

			view->addEventHandler(lensCtrl);
			view->setCameraManipulator( NULL );
		}
	}

	ViewerFrameworkImpl* CamSimulator::CreateViewerFrameworkImpl()
	{
		return new PhotoViewerImpl;
	}

	//////////////////////////////////////////////////////////////////////////
	//                     PhotoViewerImpl
	//////////////////////////////////////////////////////////////////////////
	void PhotoViewerImpl::SetupCamera(HWND hwnd, RECT rect)
	{
		// Init the GraphicsContext Traits
		Traits = new osg::GraphicsContext::Traits;
		osg::GraphicsContext::Traits* traits = Traits;

		// Init the Windata Variable that holds the handle for the Window to display OSG in.
		osg::Referenced* windata = new osgViewer::GraphicsWindowWin32::WindowData(hwnd);

		// Setup the traits parameters
		traits->x = 0;
		traits->y = 0;
		traits->width = rect.right - rect.left;
		traits->height = rect.bottom - rect.top;
		traits->windowDecoration = false;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->setInheritedWindowPixelFormat = true;
		traits->inheritedWindowData = windata;

		// Create the Graphics Context
		Gc = osg::GraphicsContext::createGraphicsContext(traits);
		osg::GraphicsContext* gc = Gc;

		//view 0, default view to see all data in the scene
		{
			osgViewer::View* view = new osgViewer::View;
			view->setName("Default View");
			this->Viewer->addView(view);

			view->getCamera()->setName("Cam 1");
			view->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
			view->getCamera()->setProjectionMatrixAsPerspective( 30.0f, (double)traits->width/(double)traits->height, 1.0f, 10000.0f );
			view->getCamera()->setGraphicsContext(gc);
			view->getCamera()->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
		}

		//view 1, photo view
		{
			osgViewer::View* view = new osgViewer::View;
			view->setName("Photo View");
			this->Viewer->addView(view);

			view->getCamera()->setName("Cam 2");
			view->getCamera()->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
			w = 320, h = 256;//TODO how to set the w and h?
			view->getCamera()->setViewport(new osg::Viewport(traits->width-w, traits->height-h, w, h));
			view->getCamera()->setGraphicsContext(gc);
			view->getCamera()->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		}

	}

	void PhotoViewerImpl::Resize(RECT rect)
	{
		osg::GraphicsContext::Traits* traits = Traits;
		traits->x = 0;
		traits->y = 0;
		traits->width = rect.right - rect.left;
		traits->height = rect.bottom - rect.top;

		{
			osgViewer::View* view = Viewer->getView(0);
			view->getCamera()->getViewport()->setViewport(
				0, 0, traits->width, traits->height);
			//view->getCamera()->setProjectionMatrixAsPerspective( 
			//	30.0f, (double)traits->width/(double)traits->height, 1.0f, 10000.0f );
		}

		{
			osgViewer::View* view = Viewer->getView(1);
			view->getCamera()->getViewport()->setViewport(
				traits->width-w, traits->height-h, w, h);
		}
	}
}


//class ImageUpdateCallBack : public osg::Drawable::UpdateCallback
//{
//public:
//	osg::Camera* camera;
//
//	virtual void update(osg::NodeVisitor* nv, osg::Drawable* drawable)
//	{
//		osg::Matrixd proj;
//		if (camera)
//		{
//			proj = camera->getProjectionMatrix();
//		}
//
//		const double near = proj(3,2) / (proj(2,2)-1.0);
//		const double far = proj(3,2) / (1.0+proj(2,2)) - 1;
//
//		const double fLeft = far * (proj(2,0)-1.0) / proj(0,0);
//		const double fRight = far * (1.0+proj(2,0)) / proj(0,0);
//		const double fTop = far * (1.0+proj(2,1)) / proj(1,1);
//		const double fBottom = far * (proj(2,1)-1.0) / proj(1,1);
//
//		osg::Geometry* geom = drawable->asGeometry();
//		osg::Vec3Array* v = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
//		(*v)[0].set( fLeft, fBottom, -far );
//		(*v)[1].set( fRight, fBottom, -far );
//		(*v)[2].set( fRight, fTop, -far );
//		(*v)[3].set( fLeft, fTop, -far );
//		v->dirty();
//	}
//};
