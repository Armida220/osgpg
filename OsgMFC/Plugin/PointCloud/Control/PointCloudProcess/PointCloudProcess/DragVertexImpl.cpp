#include "StdAfx.h"
#include "DragVertexImpl.h"
#include "DragImpl.h"

DragVertexImpl::DragVertexImpl(ostream& _log) : ControlImpl(_log)
{
	this->_dragMode = Controller::FINISH;
	this->_activeDragger = 0;

	RotateAndTranslate3DDragger* rat3d = new RotateAndTranslate3DDragger;
	rat3d->setupDefaultGeometry();//这一句不能少，否则看不见dragger
	this->m_pointsDragger = rat3d;
	this->m_pointsDragger->setName("DragVertex");
	this->m_pointsDragger->setNodeMask(0);//初始关闭dragger

	this->cmdMgr = new osgManipulator::CommandManager;

	this->virtualSel=0;
	this->sel=0;
}

void DragVertexImpl::disconnect()
{
	osg::Vec3& pt = (*dragVertexInfo.Point2Change);
	osg::Vec3 tmp = pt * virtualSel->getMatrix();
	pt.set(tmp);
	dragVertexInfo.intersection.drawable->asGeometry()->getVertexArray()->dirty();

	sel = 0;
	this->cmdMgr->disconnect(*this->m_pointsDragger);
	this->m_pointsDragger->setMatrix(osg::Matrix::identity());//这一句很关键，否则会出问题
	this->m_pointsDragger->setNodeMask(0);
	virtualSel = 0;
	log<<"Drag Vertex disconnect"<<endl;
}

bool DragVertexImpl::drag(const osgGA::GUIEventAdapter& ea, 
													osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer =
		dynamic_cast<osgViewer::View*>( &aa );
	if (!viewer)
		return false;

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
bool DragVertexImpl::connect(const osgGA::GUIEventAdapter& ea, 
														 osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer =
		dynamic_cast<osgViewer::View*>( &aa );
	if (!viewer)
		return false;

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
		dragVertexInfo.intersection = picker->getFirstIntersection();

		unsigned int idx = nodePath.size();
		while (idx--)	{
			// 查找交集节点路径中的最后一个osgManipulator::Selection
			sel =	dynamic_cast<osgManipulator::Selection*>( nodePath[ idx ] );
			if (sel == NULL)
				continue;
			if(strcmp(sel->className(), "TriangleMesh") != 0)
				continue;

			//找到selection，更新dragger
			this->m_pointsDragger->setNodeMask(1);//打开dragger

			float scale = sel->getBound().radius()/10.0;//0.01;
			RotateAndTranslate3DDragger* rad;
			rad = dynamic_cast<RotateAndTranslate3DDragger*>(m_pointsDragger.get());
			if(rad) {

				osg::Vec3Array* vArr = dynamic_cast<osg::Vec3Array*>(
					dragVertexInfo.intersection.drawable->asGeometry()->getVertexArray() );
				osg::PrimitiveSet* ps = dragVertexInfo.intersection.drawable->asGeometry()->getPrimitiveSet(
					dragVertexInfo.intersection.primitiveIndex);
				osg::DrawElementsUInt* drawUint = dynamic_cast<osg::DrawElementsUInt*>(ps);

				dragVertexInfo.intersection.drawable->asGeometry()->setUseVertexBufferObjects(true);

				osg::Vec3& v0 = vArr->at(drawUint->at(0));
				osg::Vec3& v1 = vArr->at(drawUint->at(1));
				osg::Vec3& v2 = vArr->at(drawUint->at(2));
				double len0 = (v0 - dragVertexInfo.intersection.localIntersectionPoint).length2();
				double len1 = (v1 - dragVertexInfo.intersection.localIntersectionPoint).length2();
				double len2 = (v2 - dragVertexInfo.intersection.localIntersectionPoint).length2();
				double minl = min(len0, min(len1, len2));
				if(len0==minl)
					dragVertexInfo.Point2Change = &v0;
				else if(len1==minl)
					dragVertexInfo.Point2Change = &v1;
				else if(len2==minl)
					dragVertexInfo.Point2Change = &v2;

				rad->setMatrix(scale,
					osg::Matrix::translate(
					(*dragVertexInfo.Point2Change) * sel->getMatrix()));
#if 0
				log<<"scene center:"<<scene->getBound().center().x()<<","<<scene->getBound().center().y()
					<<","<<scene->getBound().center().z()<<endl;
				log<<"sel center:"<<sel->getBound().center().x()<<","<<sel->getBound().center().y()
					<<","<<sel->getBound().center().z()<<endl;
				log<<"try to connect"<<endl;
#endif

				if(!virtualSel.valid())
					virtualSel = new osgManipulator::Selection;
				//链接dragger和selection
				if( this->cmdMgr->connect(*this->m_pointsDragger, *virtualSel ) )
					log<<"Drag Vertex connect succeed"<<endl;
				else
					log<<"Drag Vertex connect fail"<<endl;
			}

			break;
		}//end of while
	}//end of if pick

	return true;
}
bool DragVertexImpl::operator()(const osgGA::GUIEventAdapter& ea, 
																osgGA::GUIActionAdapter& aa)
{
	if(this->m_pointsDragger->getNumParents()==0) {
		osgViewer::View* viewer =
			dynamic_cast<osgViewer::View*>( &aa );
		if (!viewer)
			return false;

		osg::Group* root = dynamic_cast<osg::Group*>(viewer->getSceneData());
		if(root) {
			root->addChild(this->m_pointsDragger.get());
		}
	}

	if(virtualSel.valid()) { //有selection
		if(_dragMode==Controller::FINISH) {
			disconnect();
			return true;
		} else { //未按下tab
			return drag(ea, aa);
		}
	} else { //无selection
		if(_dragMode==Controller::BEGIN) {
			return connect(ea, aa);
		} else {
			return false;
		}
	}//end of if sel

	return false;
}
