#include "StdAfx.h"
#include "PickPointCloudImpl.h"
/*#include "CloudStation.h"*/
#include "TriangleMesh.h"
#include <iomanip>
#include "ObjectClassCode.h"
using namespace PointCloudProcess;

PickPointCloudImpl::PickPointCloudImpl(ostream& _log) : ControlImpl(_log)
{
	this->sel=0;
    _onvizselectbox = false;
    _eaStart = NULL;
    _eaEnd = NULL;
    _selectedpoints = NULL;

    _currentCloudStation = NULL;
    _dragControlor = NULL;
    _pseudSelectedObject = NULL;

    _ondragselectedpoint = false;
}

bool PickPointCloudImpl::operator()(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
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
                leaveDragSelectedPoint();
            }
            else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Delete/* || ea.getKey()==osgGA::GUIEventAdapter::KEY_BackSpace*/)
            {
                osg::notify(osg::NOTICE)<<"Delete"<<std::endl;
                DeleteSelectedPoints();
            }
            return false;
        }
    case (osgGA::GUIEventAdapter::PUSH):
        {
            _eaStart = &ea;
            std::cout<<"Mouse First: "<< _eaStart->getX() << ','<< _eaStart->getY()<<std::endl;

            if(_ondragselectedpoint && _dragControlor.valid())
                return _dragControlor->drag(ea,aa);

            if(!computeInVizSelectBox(ea,*viewer))//同时按下CTRL和鼠标左键
                return false;
            
            return true;
        }
    case (osgGA::GUIEventAdapter::DRAG):
        {
            _eaEnd = &ea;
            if(_onvizselectbox)
            {
                std::cout<<"Mouse drag: "<< _eaEnd->getX() << ','<< _eaEnd->getY()<<std::endl;
                _vizselectbox.Show(osg::Vec2(_eaStart->getX(),_eaStart->getY()),
                    osg::Vec2(_eaEnd->getX(),_eaEnd->getY()));
                
                return true;
            }

            if(_ondragselectedpoint && _dragControlor.valid())
            {    
                if(_dragControlor->drag(ea,aa))
                {
                    processfeedback();
                    return true;
                }
            }

            return false;
        }
	case (osgGA::GUIEventAdapter::RELEASE):
        {
            _eaEnd = &ea;
            
            if(!_eaStart.valid()) _eaStart = &ea;
            
            std::cout<<"Mouse Release: "<< _eaEnd->getX() << ','<< _eaEnd->getY()<<std::endl;

            bool notmov= (_eaStart->getX() == ea.getX() && _eaStart->getY() == ea.getY());
            
            if(_ondragselectedpoint && _dragControlor.valid())
            {    
                if(_dragControlor->drag(ea,aa))
                {
                    processfeedback();
                    return true;
                }
            }

            if(!notmov && _onvizselectbox) // 框选点云
            {
                processSelectBoxPick(ea,*viewer);
            }
            else if(notmov && !_onvizselectbox)//点选点云
            {
                this->sel=0;

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
                    while (idx--)	{
                        // 查找交集节点路径中的最后一个osgManipulator::Selection
                        sel =	dynamic_cast<osgManipulator::Selection*>( nodePath[ idx ] );
                        if (sel == NULL)
                            continue;

                        //点云全体高亮，应该去掉前一个的高亮
                        CloudStation* cs = dynamic_cast<CloudStation*>(sel);
                        if(cs==NULL)
                            continue;
                        cs->toggleSelect();

                        return true;
                    }
                }
            }
        }
    default:
		return false;
	}
}

bool PickPointCloudImpl::SetColorPerVertex(osg::Vec4Array& colors)
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm)
		return tm->SetColorPerVertex(colors);

	CloudStation* cs = dynamic_cast<CloudStation*>(sel);
	if(cs)
		return cs->SetColorPerVertex(colors);
	
	log<<"Error:  have not select any points!"<<endl;
	return false;
}

osg::ref_ptr<osg::Vec3Array> PickPointCloudImpl::GetTransformedPoints()
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm)
		return tm->GetTransformedPointCloud();

	CloudStation* cs = dynamic_cast<CloudStation*>(sel);
	if(cs)
		return cs->GetTransformedPointCloud();

	log<<"Error:  have not select any points!"<<endl;
	return false;
}

osg::ref_ptr<osg::Vec3Array> PickPointCloudImpl::GetSignedPoints()
{
	CloudStation* cs = dynamic_cast<CloudStation*>(sel);
	if(cs)
		return cs->GetSignedPoints();

	log<<"Error:  have not select any points!"<<endl;
	return false;
}

double PickPointCloudImpl::GetTriangleArea()
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm==0) {
		log<<"Error:  have not select any triangle mesh!"<<endl;
		return -3;
	}

	return tm->GetTriangleArea();
}

const vector<osg::DrawElementsUInt*>* PickPointCloudImpl::GetTriangleArray()
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm==0) {
		log<<"Error:  have not select any triangle mesh!"<<endl;
		return 0;
	}

	return tm->GetTriangleArray();
}

osg::Vec3Array* PickPointCloudImpl::GetVertexArray()
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm==0) {
		log<<"Error:  have not select any triangle mesh!"<<endl;
		return 0;
	}

	return tm->GetVertexArray();
}

bool PickPointCloudImpl::SetTexture(osg::Vec2Array& textureCoords, string textureName)
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm==0) {
		log<<"Error:  have not select any triangle mesh!"<<endl;
		return 0;
	}

	return tm->SetTexture(textureCoords, textureName);
}

bool PickPointCloudImpl::computeInVizSelectBox( const osgGA::GUIEventAdapter& ea,osgViewer::View& view )
{
    if( ea.getModKeyMask() == osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL &&
        ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON
        )
    {
        _onvizselectbox = true;

        if(!_vizselectbox.IsInitialized())
            _vizselectbox.Init(Utility::getGlobalHud());

        return true;
    }
    else
        _onvizselectbox = false;

    return false;
}

void PickPointCloudImpl::resetMouseposition()
{
    _eaStart = NULL;
    _eaEnd = NULL;
}

#define OUTMATRIX(out,matrix)\
{\
    out.setf(std::ios_base::left,std::ios_base::adjustfield);\
    out.setf(std::ios_base::fixed,std::ios_base::floatfield);\
    out<<"  "<<std::setw(10)<<(matrix)(0,0)<<std::setw(10)<<(matrix)(0,1)<<std::setw(10)<<(matrix)(0,2)<<std::setw(10)<<(matrix)(0,3)<<std::endl;\
    out<<"  "<<std::setw(10)<<(matrix)(1,0)<<std::setw(10)<<(matrix)(1,1)<<std::setw(10)<<(matrix)(1,2)<<std::setw(10)<<(matrix)(1,3)<<std::endl;\
    out<<"  "<<std::setw(10)<<(matrix)(2,0)<<std::setw(10)<<(matrix)(2,1)<<std::setw(10)<<(matrix)(2,2)<<std::setw(10)<<(matrix)(2,3)<<std::endl;\
    out<<"  "<<std::setw(10)<<(matrix)(3,0)<<std::setw(10)<<(matrix)(3,1)<<std::setw(10)<<(matrix)(3,2)<<std::setw(10)<<(matrix)(3,3)<<std::endl;\
    out.unsetf(std::ios_base::adjustfield);\
    out.unsetf(std::ios_base::floatfield);\
}
void PickPointCloudImpl::processSelectBoxPick( const osgGA::GUIEventAdapter& ea,osgViewer::View& view )
{
    _onvizselectbox = false;
    _vizselectbox.SetInvisible();
    
    double xMin,yMin,xMax,yMax;

    xMin = _eaStart->getXnormalized();
    xMax = _eaEnd->getXnormalized();
    if(xMin >= xMax) { double tmp=xMin; xMin = xMax; xMax = tmp; }

    yMin = _eaStart->getYnormalized();
    yMax = _eaEnd->getYnormalized();
    if( yMin >= yMax ) { double tmp=yMin; yMin = yMax; yMax = tmp; }

    osgUtil::PolytopeIntersector* picker;
    picker = new osgUtil::PolytopeIntersector( osgUtil::Intersector::PROJECTION, xMin, yMin, xMax, yMax );
    picker->setDimensionMask(osgUtil::PolytopeIntersector::DimZero);

    osgUtil::IntersectionVisitor iv(picker);

    view.getCamera()->accept(iv);

    if (picker->containsIntersections())
    {
        osg::Node* node = 0;
        CloudStation* cloudstation = 0;//首先选取的点云对象
        _selectedpointsindexs.clear();

        if(!_selectedpoints.valid()) 
            _selectedpoints = new osg::Vec3Array(picker->getIntersections().size());
        else
        {
            _selectedpoints->clear();
        }

        osg::notify(osg::NOTICE)<<"Picked Size : ============= "<<picker->getIntersections().size()<<" ================" <<std::endl;

        osgUtil::PolytopeIntersector::Intersections& intersections= picker->getIntersections();
        osgUtil::PolytopeIntersector::Intersections::iterator iter = intersections.begin();
        for (;iter !=intersections.end();iter++)
        {
            osgUtil::PolytopeIntersector::Intersection intersection = *iter;

            std::cout<<"  Picked "<<intersection.localIntersectionPoint<<std::endl
                <<"  primitive index "<<intersection.primitiveIndex
                <<"  numIntersectionPoints "<< intersection.numIntersectionPoints
                <<"  drawable object "<< intersection.drawable.get()
                <<"  Matrix is "   
                <<std::endl;
            
//             {
//                 OUTMATRIX(std::cout,*intersection.matrix)
//                 std::cout<<std::endl;
//                 std::cout<<"   Compute drawable's WorldMatrices is"<<std::endl;
//                 osg::MatrixList& matrices = intersection.drawable->getWorldMatrices();
//                 osg::Matrix mat;
//                 for (unsigned int i = 0; i < matrices.size();i++)
//                     mat = mat*matrices[i];
//                 OUTMATRIX(std::cout,mat)
//             }

            unsigned int idx = intersection.nodePath.size();
            std::cout<<"        NodePath size"<<idx<<std::endl;
            
            CloudStation* tmpcloudstation = 0;

            while (idx--)
            {
                // 查找交集节点路径中的最后一个osgManipulator::Selection
                node = intersection.nodePath[idx];

//                 std::cout<<"            Hits "<<idx << ' '<<node->className()<<" object "<<node<<std::endl;
                // ToDo will use NODEMASK for judgemenet
                if(strcmp(node->className(), "CloudStation") != 0)
                    continue;

                tmpcloudstation = dynamic_cast<CloudStation*>(node);
                if (tmpcloudstation == NULL)
                    continue;

                break;
            }

            //只判断在一个点云内选中的点，如果选中两个点云中的点，这里必须确定唯一一个点云。
            if(!cloudstation) 
            {
                if(tmpcloudstation)
                    cloudstation = tmpcloudstation;
                else
                    continue;
            }
            else
            {
                if(cloudstation != tmpcloudstation)
                    continue;
            }

            typedef osg::Geometry::PrimitiveSetList PSetList;

            PSetList& ps = intersection.drawable->asGeometry()->getPrimitiveSetList();
//             std::cout<< "    Intersection Drawable As Geometry has " << intersection.drawable->asGeometry()->getNumPrimitiveSets() 
//                 << " primitiveset;" << std::endl;

            for(PSetList::iterator prit = ps.begin(); prit != ps.end(); prit++)
            {
//                 std::cout<<"    Each primitiveSet's Type is "<< (*prit)->className()<<std::endl;
                if(strcmp((*prit)->className(),"DrawArrays") == 0)
                {
                    osg::Vec3Array* vArr = dynamic_cast<osg::Vec3Array*>(intersection.drawable->asGeometry()->getVertexArray());
                    _selectedpoints->push_back(vArr->at(intersection.primitiveIndex));
                    _selectedpointsindexs.push_back(GeometryPrimitiveIndexPair(intersection.primitiveIndex,intersection.drawable->asGeometry()));
                }
            }

             osg::Vec3Array* vArr = dynamic_cast<osg::Vec3Array*>(intersection.drawable->asGeometry()->getVertexArray() );
             std::cout<<"\tVertex Array size "<<vArr->size()<<std::endl << "\tWhether equal to Picked ? " << vArr->at(intersection.primitiveIndex) <<std::endl;
            
        }
        
        //高亮选中的点并显示出拖拽界面
        _currentCloudStation = cloudstation;
        osg::Node* dragandsceneobject = enterDragSelectedPoint();
        if(dragandsceneobject)
        {
            _pseudSelectedObject = cloudstation;/*view.getSceneData()->asGroup();*/
            _pseudSelectedObject->addChild(dragandsceneobject);
            _ondragselectedpoint = true;
        }
    }
}

osg::Node* PickPointCloudImpl::enterDragSelectedPoint( void )
{
    if(!_selectedpoints.valid()) return NULL;

    if(_selectedpoints->empty()) return NULL;
    
    // ToDo will configure pointgeom's color
    osg::ref_ptr<osg::Geode> pointgeom = Utility::createPointGeode(_selectedpoints,osg::Vec4(0.0f,1.0f,0.0,1.0));

    if(pointgeom.valid())
    {
        _dragControlor = NULL;
        _dragControlor = new CommDragImpl();
        
        osg::Group* root = _dragControlor->createdrag(pointgeom.get());

        return root;
    }

    return NULL;
}

void PickPointCloudImpl::processfeedback( void )
{
    if(_ondragselectedpoint)
    {
        osg::Matrix mat= _dragControlor->getTransformMatrix();
        
        unsigned int index = 0;
        osg::Geometry* geom = NULL;
        
        unsigned int originalvalueindex = 0;
        static bool first = true;

        GroupGeometryIndexsV::iterator iter = _selectedpointsindexs.begin();
        for (;iter != _selectedpointsindexs.end();iter++,originalvalueindex++)
        {
             index = (*iter).first;
             geom = (*iter).second.get();

             osg::Vec3Array* vArr = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
             if(first)
             {
                 std::cout<<"the first processfeedback,check qual " 
                     <<(_selectedpoints->at(originalvalueindex) == vArr->at(index)?"Yes":"No")<<std::endl;
                 first = false;
                 vArr->at(index) = vArr->at(index)*mat;
             }
             else
                 vArr->at(index) = _selectedpoints->at(originalvalueindex)*mat;/*vArr->at(index)*mat;*/
             
             geom->dirtyBound();
             geom->dirtyDisplayList();
        }
    }
}

void PickPointCloudImpl::leaveDragSelectedPoint()
{
    if(_ondragselectedpoint)
    {
        _ondragselectedpoint = false;
        _dragControlor->stop();
        Utility::DeleteSelectedNodesVisitor dsn(CLASSCODE::PseudSelectedPoint);
        if(_pseudSelectedObject.valid()) _pseudSelectedObject->accept(dsn);
        dsn.pruneSelectedNodes();
        _pseudSelectedObject = NULL;
    }
}

void PickPointCloudImpl::DeleteSelectedPoints()
{
    leaveDragSelectedPoint();

    /*_currentCloudStation->DeletePart(_selectedpointsindexs);*/

    _selectedpointsindexs.clear();
    if(_selectedpoints.valid())_selectedpoints->clear();
}