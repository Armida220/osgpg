#include "StdAfx.h"
#include "DragImpl.h"

DragImpl::DragImpl(ostream& _log) : ControlImpl(_log)
{
	this->_dragMode = Controller::FINISH;
	this->_activeDragger = 0;

	RotateAndTranslate3DDragger* rat3d = new RotateAndTranslate3DDragger;
	rat3d->setupDefaultGeometry();
	this->m_pointsDragger = rat3d;
	this->m_pointsDragger->setName("Drag");
	this->m_pointsDragger->setNodeMask(0);//初始关闭dragger

	this->cmdMgr = new MyCommandManager;

	this->sel = 0;
}

void DragImpl::disconnect()
{
	sel = 0;
	this->cmdMgr->disconnect(*this->m_pointsDragger);
	this->m_pointsDragger->setMatrix(osg::Matrix::identity());//这一句很关键，否则会出问题
	this->m_pointsDragger->setNodeMask(0);
	log<<"Drag disconnect"<<endl;
}

bool DragImpl::drag(const osgGA::GUIEventAdapter& ea,
										osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
	if (!viewer) return false;

	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::PUSH:
		{
			osgUtil::LineSegmentIntersector::Intersections intersections;

			_pointer.reset();

			if (viewer->computeIntersections(ea.getX(),ea.getY(),intersections))
			{
				_pointer.setCamera(viewer->getCamera());
				_pointer.setMousePosition(ea.getX(), ea.getY());

				for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
					hitr != intersections.end();
					++hitr)
				{
					_pointer.addIntersection(hitr->nodePath, hitr->getLocalIntersectPoint());
				}
				for (osg::NodePath::iterator itr = _pointer._hitList.front().first.begin();
					itr != _pointer._hitList.front().first.end();
					++itr)
				{
					osgManipulator::Dragger* dragger = dynamic_cast<osgManipulator::Dragger*>(*itr);
					if (dragger)
					{
						dragger->handle(_pointer, ea, aa);
						_activeDragger = dragger;
						return true;
					}                   
				}
			}
			return false;//__super::handle(ea,aa);
		}
	case osgGA::GUIEventAdapter::DRAG:
	case osgGA::GUIEventAdapter::RELEASE:
		{
			if (_activeDragger)
			{
				_pointer._hitIter = _pointer._hitList.begin();
				_pointer.setCamera(viewer->getCamera());
				_pointer.setMousePosition(ea.getX(), ea.getY());

				_activeDragger->handle(_pointer, ea, aa);

				if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE)
				{
					_activeDragger = 0;
					_pointer.reset();
				}

				return true;
			}
			return false;//__super::handle(ea,aa);
		}
	default:
		return false;//__super::handle(ea,aa);
	}
}


bool DragImpl::connect(const osgGA::GUIEventAdapter& ea,
											 osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
	if (!viewer) return false;

	if(ea.getEventType() != osgGA::GUIEventAdapter::RELEASE)
		return false;

	double x = ea.getXnormalized();
	double y = ea.getYnormalized();

	osgUtil::PolytopeIntersector* picker =
		new osgUtil::PolytopeIntersector(
		osgUtil::Intersector::PROJECTION,
		x-0.01, y-0.01, x+0.01, y+0.01 );

	//求交寻找selection
	osgUtil::IntersectionVisitor iv( picker );
	viewer->getCamera()->accept( iv );

	if (picker->containsIntersections()) {
		osg::NodePath& nodePath =
			picker->getFirstIntersection().nodePath;
		unsigned int idx = nodePath.size();
		while (idx--)	{
			// 查找交集节点路径中的最后一个osgManipulator::Selection
			sel =	dynamic_cast<osgManipulator::Selection*>( nodePath[ idx ] );
			if (sel == NULL)
				continue;

			//找到selection，更新dragger
			this->m_pointsDragger->setNodeMask(1);//打开dragger
			osg::Node* scene=0;
			if(sel->getNumChildren()>1)
				scene = sel->getChild(1);
			if(scene == 0)
				break;

			float scale = scene->getBound().radius();
			RotateAndTranslate3DDragger* rad;
			rad = dynamic_cast<RotateAndTranslate3DDragger*>(m_pointsDragger.get());
			if(rad) {
				rad->setMatrix(scale,
					osg::Matrix::translate(sel->getBound().center()));

#if 0
				log<<"scene center:"<<scene->getBound().center().x()<<","<<scene->getBound().center().y()
					<<","<<scene->getBound().center().z()<<endl;
				log<<"sel center:"<<sel->getBound().center().x()<<","<<sel->getBound().center().y()
					<<","<<sel->getBound().center().z()<<endl;
				log<<"try to connect"<<endl;
#endif
				//链接dragger和selection
				if( this->cmdMgr->connect(*this->m_pointsDragger, *sel) )
					log<<"Drag connect succeed"<<endl;
				else
					log<<"Drag connect fail"<<endl;
			}

			break;
		}//end of while
	}//end of if pick

	return true;
}
bool DragImpl::operator()(const osgGA::GUIEventAdapter& ea, 
													osgGA::GUIActionAdapter& aa)
{
	if(this->m_pointsDragger->getNumParents()==0) {
		osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
		if (!viewer) return false;

		osg::Group* root = dynamic_cast<osg::Group*>(viewer->getSceneData());
		if(root) {
			root->addChild(this->m_pointsDragger.get());
		}
	}

	if(sel) { //有selection
		if(_dragMode==Controller::FINISH) {
			disconnect();
			return false;
		} else { //未按下tab
			return drag(ea, aa);
		}
	} else { //无selection
		if(_dragMode==Controller::BEGIN) {
			return connect(ea, aa);
		} else {
			return false;//__super::handle(ea, aa);
		}
	}//end of if sel

	return false;//__super::handle(ea,aa);
}