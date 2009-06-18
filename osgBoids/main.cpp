#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osg/NodeVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <iostream>

#include "BoidsManager.h"

using namespace std;
using namespace osgMA;

class AnimationCallBack : public osg::NodeCallback
{
protected:
	BoidsManager* _bm;
public:
	AnimationCallBack(BoidsManager& bm) { _bm = &bm; }

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		static int cnt=0;
		if(cnt==10)
			_bm->Frame(), cnt=0;
		else
			cnt++;
		traverse(node,nv);
	}
};

class AnimationController : public osgGA::GUIEventHandler
{
protected:
	BoidsManager* _bm;
public:
	AnimationController(BoidsManager& bm) { _bm = &bm; }

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& ga)
	{ 
		if(ea.getEventType()==osgGA::GUIEventAdapter::KEYUP)
		{
			_bm->Frame();
		}
		return false; 
	}
};


int main(int argc, char** argv)
{
	BoidsManager bm;
	bm.SetModelName(string(argv[1]));
	bm.Init_positions();

	osg::ref_ptr<osg::Group> root = bm.GetRoot(); 
	root->setUpdateCallback(new AnimationCallBack(bm));

	cout<<root->getBound().radius()<<endl;
	system("pause");
	
	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	//viewer.addEventHandler(new AnimationController(bm));
		
	return viewer.run();
}
