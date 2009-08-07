#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>
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
#include "CamCalibrator.h"
#include <iostream>
using namespace std;

bool g_flag;

class LensController : public osgGA::GUIEventHandler
{
	osg::Camera* _cam;
	double t,b,l,r,n,f;
public:
	void SetCameraToControl(osg::Camera* cam) { _cam = cam; }
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
			if(ea.getKey()=='y') { g_flag = true; }
			if(ea.getKey()=='u') { g_flag = false; }
			return true;
		}
		return false;
	}
}; 

class ImageUpdateCallBack : public osg::Drawable::UpdateCallback
{
public:
	osg::Camera* camera;

	virtual void update(osg::NodeVisitor* nv, osg::Drawable* drawable)
	{
		if(g_flag) { return; }
		osg::Matrixd proj;
		osg::Matrixd mv;
		if (camera)
		{
			proj = camera->getProjectionMatrix();
			mv = camera->getViewMatrix();
		}

		const double near = proj(3,2) / (proj(2,2)-1.0);
		const double far = proj(3,2) / (1.0+proj(2,2)) - 1;

		const double fLeft = far * (proj(2,0)-1.0) / proj(0,0);
		const double fRight = far * (1.0+proj(2,0)) / proj(0,0);
		const double fTop = far * (1.0+proj(2,1)) / proj(1,1);
		const double fBottom = far * (proj(2,1)-1.0) / proj(1,1);

		osg::Geometry* geom = drawable->asGeometry();
		osg::Vec3Array* v = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
		(*v)[0].set( fLeft, fBottom, -far );
		(*v)[1].set( fRight, fBottom, -far );
		(*v)[2].set( fRight, fTop, -far );
		(*v)[3].set( fLeft, fTop, -far );
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

	const double near = proj(3,2) / (proj(2,2)-1.0);
	const double far = proj(3,2) / (1.0+proj(2,2)) - 1;

	const double fLeft = far * (proj(2,0)-1.0) / proj(0,0);
	const double fRight = far * (1.0+proj(2,0)) / proj(0,0);
	const double fTop = far * (1.0+proj(2,1)) / proj(1,1);
	const double fBottom = far * (proj(2,1)-1.0) / proj(1,1);

	osg::Vec3Array* v = new osg::Vec3Array;
	v->resize( 4 );
	(*v)[0].set( fLeft, fBottom, -far );
	(*v)[1].set( fRight, fBottom, -far );
	(*v)[2].set( fRight, fTop, -far );
	(*v)[3].set( fLeft, fTop, -far );

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
	//texture->setDataVariance(osg::Object::DYNAMIC);
	texture->setImage(image);
	stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

	ImageUpdateCallBack* imgcb = new ImageUpdateCallBack;
	imgcb->camera = camera;
	geom->setUpdateCallback( imgcb );

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

	const double near = proj(3,2) / (proj(2,2)-1.0);
	const double far = proj(3,2) / (1.0+proj(2,2));

	const double nLeft = near * (proj(2,0)-1.0) / proj(0,0);
	const double nRight = near * (1.0+proj(2,0)) / proj(0,0);
	const double nTop = near * (1.0+proj(2,1)) / proj(1,1);
	const double nBottom = near * (proj(2,1)-1.0) / proj(1,1);

	const double fLeft = far * (proj(2,0)-1.0) / proj(0,0);
	const double fRight = far * (1.0+proj(2,0)) / proj(0,0);
	const double fTop = far * (1.0+proj(2,1)) / proj(1,1);
	const double fBottom = far * (proj(2,1)-1.0) / proj(1,1);

	osg::Vec3Array* v = new osg::Vec3Array;
	v->resize( 9 );
	(*v)[0].set( 0., 0., 0. );
	(*v)[1].set( nLeft, nBottom, -near );
	(*v)[2].set( nRight, nBottom, -near );
	(*v)[3].set( nRight, nTop, -near );
	(*v)[4].set( nLeft, nTop, -near );
	(*v)[5].set( fLeft, fBottom, -far );
	(*v)[6].set( fRight, fBottom, -far );
	(*v)[7].set( fRight, fTop, -far );
	(*v)[8].set( fLeft, fTop, -far );

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

	osg::Geode* geode = new osg::Geode;
	geode->addDrawable( geom );

	geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );

	osg::MatrixTransform* mt = new osg::MatrixTransform;
	mt->setMatrix( osg::Matrixd::inverse( mv ) );
	mt->addChild( geode );

	return mt;
}

void ResetCamera(osg::Camera* cam, osg::BoundingSphere bs)
{
	//step 1 read K, world points, image points
	FC::K_Matrix k;
	FC::Uint correspondentNum;
	FC::WorldPoint world[100];
	FC::ImagePoint image[100];
	cout<<"输入K矩阵参数："<<endl;
	cout<<"alpha"<<endl;
	cin>>k[0]; k[1] = k[0];
	cout<<"u0 v0"<<endl;
	cin>>k[2]>>k[3];

	cout<<"控制点数目"<<endl;
	cin>>correspondentNum;

	cout<<"物方点 像方点"<<endl;
	for(FC::Uint i=0; i<correspondentNum; ++i) {
		FC::WorldPoint& wp = world[i];
		FC::ImagePoint& ip = image[i];
		cin>>wp[0]>>wp[1]>>wp[2]>>ip[0]>>ip[1];
	}
	//step 2 calibrate
	FC::R_Matrix r;
	FC::C_Matrix c;
	FC::PrincipleAxis pa;
	FC::P_Matrix p;
	FC::calibrate(k, correspondentNum, image, world, 
		r, c, pa, p);

// 	osg::Matrix rot;
// 	rot.set(r[0][0], r[0][1], r[0][2], 0,
// 					r[1][0], r[1][1], r[1][2], 0,
// 					r[2][0], r[2][1], r[2][2], 0,
// 						 0,		0,		 0, 1);
	osg::Vec3 eye = osg::Vec3(c[0], c[1], c[2]);
	osg::Vec3 look = osg::Vec3(pa[0], pa[1], pa[2]);
	osg::Vec3 up = osg::Vec3(-r[1][0], -r[1][1], -r[1][2]);
	//step 3 reset camera
	cam->setViewMatrixAsLookAt(eye, eye + look, up	);
	
	double f = 31.03, dx = 0.007;
	double lx = 640, ly = 512;
	osg::Viewport* viewport = cam->getViewport();
	double znear, zfar, left, right, top, bottom;
	znear = f;
	zfar = znear + bs.radius()*2 + 1;
	left = -dx*lx/f*znear;
	right = -left;
	top = dx*ly/f*znear;
	bottom = -top;
	cam->setProjectionMatrixAsFrustum(
		left, right, bottom, top, znear, zfar);
}

int main(int argc, char** argv)
{
	g_flag = false;
	setlocale(LC_ALL, "chs");
	osg::ArgumentParser arguments( &argc, argv );

	osg::ref_ptr< osg::Group > root = new osg::Group;

	// Child 0: We'll replace this every frame with an updated representation
	//   of the view frustum.
	root->addChild( makeFrustumFromCamera( NULL ) );
	osg::Node* axes = osgDB::readNodeFile(string("axes.osg"));
	root->addChild( axes );
	root->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );

	cout<<"点模型文件："<<endl;
	osg::ref_ptr< osg::Node > scenePoints;
	string pointPath;
	cin>>pointPath;
	scenePoints = osgDB::readNodeFile( pointPath );
	if (!scenePoints.valid())
	{
		cout<<"无法读入模型!"<<endl;
		return 1;
	}

	osgViewer::CompositeViewer viewer( arguments );
	// Turn on FSAA, makes the lines look better.
	osg::DisplaySettings::instance()->setNumMultiSamples( 4 );

	// Create View 0 -- Just the loaded model.
	string imagePath;
	osg::Group* gp = new osg::Group;
	osg::ref_ptr<osg::Image> image;
	{
		osgViewer::View* view = new osgViewer::View;
		viewer.addView( view );
		view->setUpViewInWindow( 10, 10, 320, 200 );
		view->getCamera()->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
		//////////////////////////////////////////////////////////////////////////
		//step 1 calibration
		osg::Camera* cam = view->getCamera();
		ResetCamera(cam, scenePoints->getBound());

		//step 2 贴图
		cout<<"图片文件："<<endl;
		cin>>imagePath;

		image = osgDB::readImageFile(imagePath);
		osg::ref_ptr< osg::Node > sceneImage;
		sceneImage = makeImageFromCamera(cam, image);
		gp->addChild(sceneImage);
		gp->addChild(scenePoints);
		view->setSceneData( gp );
		root->addChild( gp );
		//////////////////////////////////////////////////////////////////////////
		LensController* lensCtrl = new LensController;
		lensCtrl->SetCameraToControl(view->getCamera());
		view->addEventHandler(lensCtrl);
		view->setCameraManipulator( NULL );
	}

	// Create view 1 -- Contains the loaded moel, as well as a wireframe frustum derived from View 0's Camera.
	{
		osgViewer::View* view = new osgViewer::View;
		viewer.addView( view );

		view->setUpViewInWindow( 10, 510, 640, 480 );
		view->setSceneData( root.get() );
		view->setCameraManipulator( new osgGA::TrackballManipulator );
	}

	while (!viewer.done())
	{
		// Update the wireframe frustum
		root->removeChild( 0, 1 );
		root->insertChild( 0, makeFrustumFromCamera(
			viewer.getView( 0 )->getCamera() ) );

// 		gp->removeChild( 0, 1 );
// 		gp->insertChild( 0, makeImageFromCamera(
// 			viewer.getView( 0 )->getCamera(), image ) );

		viewer.frame();
	}
	return 0;
}
