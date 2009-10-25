#include "StdAfx.h"
#include "PickTriangleImpl.h"
#include "Utility.h"
#include "ObjectClassCode.h"
using namespace PointCloudProcess;

PickTriangleImpl::PickTriangleImpl(ostream& _log) : ControlImpl(_log)
{
}

bool PickTriangleImpl::operator()(const osgGA::GUIEventAdapter& ea,
																						osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer =
		dynamic_cast<osgViewer::View*>( &aa );
	if (!viewer)
		return false;

	switch(ea.getEventType())
	{
	case osgGA::GUIEventAdapter::RELEASE:

		double x = ea.getXnormalized();
		double y = ea.getYnormalized();

		osgUtil::PolytopeIntersector* picker =
			new osgUtil::PolytopeIntersector(
			osgUtil::Intersector::PROJECTION,
			x-0.01, y-0.01, x+0.01, y+0.01 );

		osgUtil::IntersectionVisitor iv( picker );
		viewer->getCamera()->accept( iv );

		if (picker->containsIntersections())
		{
			osgUtil::PolytopeIntersector::Intersection& intersection = 
				picker->getFirstIntersection();

			osg::NodePath& nodePath =
				intersection.nodePath;
			unsigned int idx = nodePath.size();
			osgManipulator::Selection* sel=0;
			while (idx--)	{
				// 查找交集节点路径中的最后一个osgManipulator::Selection
				sel =	dynamic_cast<osgManipulator::Selection*>( nodePath[ idx ] );
				if (sel == NULL)
					continue;
				if( strcmp(sel->className(), "TriangleMesh") != 0 )
					continue;

				osg::IntArray* iA = dynamic_cast<osg::IntArray*>(
					intersection.drawable->asGeometry()->getColorIndices() );

				if(iA)
					(*iA)[intersection.primitiveIndex] = !(*iA)[intersection.primitiveIndex];

				intersection.drawable->dirtyDisplayList();

				triangle.pickedPoint = intersection.localIntersectionPoint;
				osg::Vec3Array* vArr = dynamic_cast<osg::Vec3Array*>(
					intersection.drawable->asGeometry()->getVertexArray() );
				osg::DrawElementsUInt* drawIdx = dynamic_cast<osg::DrawElementsUInt*>(
					intersection.drawable->asGeometry()->getPrimitiveSet(
					intersection.primitiveIndex) );
				if(vArr && drawIdx && drawIdx->size()==3) {
					GLuint id0 = drawIdx->at(0);
					GLuint id1 = drawIdx->at(1);
					GLuint id2 = drawIdx->at(2);

					triangle.vertexs[0] = vArr->at(id0);
					triangle.vertexs[1] = vArr->at(id1);
					triangle.vertexs[2] = vArr->at(id2);
				}

				return true;
			}
		}

		return true;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
// PickTriangleEdgeImpl 选中三角形边或者可以拖动
bool PickTriangleEdgeImpl::operator()( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    osgViewer::View* viewer =
        dynamic_cast<osgViewer::View*>( &aa );
    if (!viewer)
        return false;

    switch(ea.getEventType())
    {
    case(osgGA::GUIEventAdapter::KEYUP):
        {
            if (ea.getKey()== osgGA::GUIEventAdapter::KEY_BackSpace)
            {
                cancelSelectedEdgeDrag();
            }
            else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Return/* || ea.getKey()==osgGA::GUIEventAdapter::KEY_BackSpace*/)
            {
                saveSelectedEdgeDragState();
            }
            return false;
        }
    case (osgGA::GUIEventAdapter::PUSH):
        {
            if(_ondragselected && _dragControlor.valid())
                return _dragControlor->drag(ea,aa);

            return true;
        }
    case (osgGA::GUIEventAdapter::DRAG):
        {
            if(_ondragselected && _dragControlor.valid())
            {    
                if(_dragControlor->drag(ea,aa))
                {
                    processfeedback();
                    return true;
                }
            }

            return false;
        }
    case osgGA::GUIEventAdapter::RELEASE:

        if(_ondragselected && _dragControlor.valid())
        {    
            if(_dragControlor->drag(ea,aa))
            {
                processfeedback();
                
            }
            return true;
        }
        double x = ea.getXnormalized();
        double y = ea.getYnormalized();

        osgUtil::PolytopeIntersector* picker =
            new osgUtil::PolytopeIntersector(
            osgUtil::Intersector::PROJECTION,
            x-0.01, y-0.01, x+0.01, y+0.01 );
        //picker->setDimensionMask(osgUtil::PolytopeIntersector::DimOne);

        osgUtil::IntersectionVisitor iv( picker );
        viewer->getCamera()->accept( iv );

        if (picker->containsIntersections()) {
            osg::NodePath& nodePath =
                picker->getFirstIntersection().nodePath;
            osgUtil::PolytopeIntersector::Intersection& intersection = picker->getFirstIntersection();

            unsigned int idx = nodePath.size();
            while (idx--)	{
                // 查找交集节点路径中的最后一个osgManipulator::Selection
                _triMesh = dynamic_cast<TriangleMesh*>(nodePath[ idx ] );
                if (_triMesh == NULL)
                    continue;

                osg::Vec3Array* vArr = dynamic_cast<osg::Vec3Array*>(
                    intersection.drawable->asGeometry()->getVertexArray() );
                osg::PrimitiveSet* ps = intersection.drawable->asGeometry()->getPrimitiveSet(
                    intersection.primitiveIndex);
                osg::DrawElementsUInt* drawUint = dynamic_cast<osg::DrawElementsUInt*>(ps);

                intersection.drawable->asGeometry()->setUseVertexBufferObjects(true);

                osg::Vec3& v0 = vArr->at(drawUint->at(0));
                osg::Vec3& v1 = vArr->at(drawUint->at(1));
                osg::Vec3& v2 = vArr->at(drawUint->at(2));
                double len0 = (v0 - intersection.localIntersectionPoint).length2();
                double len1 = (v1 - intersection.localIntersectionPoint).length2();
                double len2 = (v2 - intersection.localIntersectionPoint).length2();
                double maxl = max(len0, max(len1, len2));
                if(len0==maxl)
                {
                    orgp0.set(v1); 
                    p0 = &v1;
                    orgp1.set(v2);
                    p1 = &v2;
                }
                else if(len1==maxl)
                {
                    orgp0.set(v0); p0 = &v0;
                    orgp1.set(v2); p1 = &v2;
                }
                else if(len2==maxl)
                {
                    orgp0 = v0; p0 = &v0;
                    orgp1 = v1; p1 = &v1;
                }
                _geom = intersection.drawable->asGeometry();
                
                {
                    osg::Node* dragandsceneobject = enterDragSelected();
                    if (dragandsceneobject)
                        _triMesh->addChild(dragandsceneobject);
                    _ondragselected = true;
                }
                break;
            }//end of while
        }//end of if pick
        return true;
    }

    return true;
}

PickTriangleEdgeImpl::PickTriangleEdgeImpl( ostream& _log/*=cout*/ ): ControlImpl(_log)
{
    p0 = p1= NULL;
    _triMesh = NULL;
    _geom = NULL;
}

void PickTriangleEdgeImpl::processfeedback( void )
{
    if(_ondragselected)
    {
        osg::Matrix mat= _dragControlor->getTransformMatrix();

        p0->set(orgp0*mat);
        p1->set(orgp1*mat);

        _geom->getVertexArray()->dirty();
        //_geom->dirtyDisplayList();
    }
}

void PickTriangleEdgeImpl::cancelSelectedEdgeDrag()
{
    if(_ondragselected)
    {
        p0->set(orgp0);
        p1->set(orgp1);

        _geom->getVertexArray()->dirty();

        saveSelectedEdgeDragState();
    }
}

void PickTriangleEdgeImpl::saveSelectedEdgeDragState()
{
    if (_ondragselected)
    {
        _ondragselected = false;
        _dragControlor->stop();

        Utility::DeleteSelectedNodesVisitor dsn(CLASSCODE::PseudSelectedLine);
        _triMesh->accept(dsn);
        dsn.pruneSelectedNodes();

        p0 = p1= NULL;
        _triMesh = NULL;
        _geom = NULL;
    }
}

osg::Node* PickTriangleEdgeImpl::enterDragSelected( void )
{
    if(!p0 || !p1) return NULL;

    osg::ref_ptr<osg::Vec3Array> vetarry = new osg::Vec3Array();
    vetarry->push_back(orgp0);
    vetarry->push_back(orgp1);
    
    osg::ref_ptr<osg::Geode> pointgeom = Utility::createLineGeode(vetarry,osg::Vec4(1.0f,0.0f,0.5f,1.0));

    if(pointgeom.valid())
    {
        _dragControlor = NULL;
        _dragControlor = new CommDragImpl();

        osg::Group* root = _dragControlor->createdrag(pointgeom.get());

        return root;
    }

    return NULL;
}
//////////////////////////////////////////////////////////////////////////
PickTriangleFaceImpl::PickTriangleFaceImpl( ostream& _log/*=cout*/ ): PickTriangleEdgeImpl(_log)
{
    p2 = NULL;
}

bool PickTriangleFaceImpl::operator()( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    osgViewer::View* viewer =
        dynamic_cast<osgViewer::View*>( &aa );
    if (!viewer)
        return false;

    switch(ea.getEventType())
    {
    case(osgGA::GUIEventAdapter::KEYUP):
        {
            if (ea.getKey()== osgGA::GUIEventAdapter::KEY_BackSpace)
            {
                cancelSelectedFaceDrag();
            }
            else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Return/* || ea.getKey()==osgGA::GUIEventAdapter::KEY_BackSpace*/)
            {
                saveSelectedFaceDragState();
            }
            return false;
        }
    case (osgGA::GUIEventAdapter::PUSH):
        {
            if(_ondragselected && _dragControlor.valid())
                return _dragControlor->drag(ea,aa);

            return true;
        }
    case (osgGA::GUIEventAdapter::DRAG):
        {
            if(_ondragselected && _dragControlor.valid())
            {    
                if(_dragControlor->drag(ea,aa))
                {
                    processfeedback();
                    return true;
                }
            }

            return false;
        }
    case osgGA::GUIEventAdapter::RELEASE:

        if(_ondragselected && _dragControlor.valid())
        {    
            if(_dragControlor->drag(ea,aa))
            {
                processfeedback();
                
            }
            return true;
        }
        double x = ea.getXnormalized();
        double y = ea.getYnormalized();

        osgUtil::PolytopeIntersector* picker =
            new osgUtil::PolytopeIntersector(
            osgUtil::Intersector::PROJECTION,
            x-0.01, y-0.01, x+0.01, y+0.01 );
        //picker->setDimensionMask(osgUtil::PolytopeIntersector::DimTwo);

        osgUtil::IntersectionVisitor iv( picker );
        viewer->getCamera()->accept( iv );

        if (picker->containsIntersections()) 
        {
            osg::NodePath& nodePath =
                picker->getFirstIntersection().nodePath;
            osgUtil::PolytopeIntersector::Intersection& intersection = picker->getFirstIntersection();

            unsigned int idx = nodePath.size();
            while (idx--)	{
                // 查找交集节点路径中的最后一个osgManipulator::Selection
                _triMesh = dynamic_cast<TriangleMesh*>(nodePath[ idx ] );
                if (_triMesh == NULL)
                    continue;

                osg::Vec3Array* vArr = dynamic_cast<osg::Vec3Array*>(
                    intersection.drawable->asGeometry()->getVertexArray() );
                osg::PrimitiveSet* ps = intersection.drawable->asGeometry()->getPrimitiveSet(
                    intersection.primitiveIndex);
                osg::DrawElementsUInt* drawUint = dynamic_cast<osg::DrawElementsUInt*>(ps);

                intersection.drawable->asGeometry()->setUseVertexBufferObjects(true);

                osg::Vec3& v0 = vArr->at(drawUint->at(0));
                osg::Vec3& v1 = vArr->at(drawUint->at(1));
                osg::Vec3& v2 = vArr->at(drawUint->at(2));
                orgp0 = v0; p0 = &v0;
                orgp1 = v1; p1 = &v1;
                orgp2 = v2; p2 = &v2;
                
                _geom = intersection.drawable->asGeometry();

                {
                    osg::Node* dragandsceneobject = enterDragSelected();
                    if (dragandsceneobject)
                        _triMesh->addChild(dragandsceneobject);
                    _ondragselected = true;
                }
                break;
            }//end of while
        }//end of if pick
        return true;
    }

    return true;
}

void PickTriangleFaceImpl::processfeedback( void )
{
    if(_ondragselected)
    {
        osg::Matrix mat= _dragControlor->getTransformMatrix();

        p0->set(orgp0*mat);
        p1->set(orgp1*mat);
        p2->set(orgp2*mat);

        _geom->getVertexArray()->dirty();
    }
}

void PickTriangleFaceImpl::cancelSelectedFaceDrag()
{
    if(_ondragselected)
    {
        p0->set(orgp0);
        p1->set(orgp1);
        p2->set(orgp2);

        _geom->getVertexArray()->dirty();

        saveSelectedEdgeDragState();
    }
}

void PickTriangleFaceImpl::saveSelectedFaceDragState()
{
    if (_ondragselected)
    {
        _ondragselected = false;
        _dragControlor->stop();

        Utility::DeleteSelectedNodesVisitor dsn(CLASSCODE::PseudSelectedFace);
        _triMesh->accept(dsn);
        dsn.pruneSelectedNodes();

        p0 = p1= p2= NULL;
        _triMesh = NULL;
        _geom = NULL;
    }
}

osg::Node* PickTriangleFaceImpl::enterDragSelected( void )
{
    if(!p0 || !p1 || !p2) return NULL;

    osg::ref_ptr<osg::Vec3Array> vetarry = new osg::Vec3Array();
    vetarry->push_back(orgp0);
    vetarry->push_back(orgp1);
    vetarry->push_back(orgp2);

    osg::ref_ptr<osg::Geode> pointgeom = Utility::createTriGeode(vetarry,osg::Vec4(1.0f,0.0f,0.5f,1.0));

    if(pointgeom.valid())
    {
        _dragControlor = NULL;
        _dragControlor = new CommDragImpl();

        osg::Group* root = _dragControlor->createdrag(pointgeom.get());

        return root;
    }

    return NULL;
}