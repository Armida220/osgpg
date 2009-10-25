#include "StdAfx.h"
#include "LabelPointImpl.h"
#include "ObjectClassCode.h"
#include "CloudStation.h"
#include "DataCallBack.h"
using namespace PointCloudProcess;

LabelPointImpl::LabelPointImpl(ostream& _log) : ControlImpl(_log)
{
	this->curID=0;
    _selectedSignObject = NULL;
}

bool LabelPointImpl::operator()(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer =
		dynamic_cast<osgViewer::View*>( &aa );
	if (!viewer)
		return false;

	static double x;
	static double y;

	switch(ea.getEventType())
	{
    case(osgGA::GUIEventAdapter::KEYUP):
        {
           if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Delete)
            {
                osg::notify(osg::NOTICE)<<"Delete "<< curID <<" SignObject"<<std::endl;
                DeleteSelectedSignObject();
            }
            return false;
        }
	case osgGA::GUIEventAdapter::PUSH:
		x = ea.getXnormalized();
		y = ea.getYnormalized();
		return true;

	case osgGA::GUIEventAdapter::RELEASE:

		double nx = ea.getXnormalized();
		double ny = ea.getYnormalized();

		if(!(x==nx && y==ny))
			return true;

		osgUtil::PolytopeIntersector* picker =
			new osgUtil::PolytopeIntersector(
			osgUtil::Intersector::PROJECTION,
			x-0.01, y-0.01, x+0.01, y+0.01 );

		osgUtil::IntersectionVisitor iv( picker );
		viewer->getCamera()->accept( iv );

		if (picker->containsIntersections())
		{
//             {
//                 osgUtil::PolytopeIntersector::Intersections& intersections= picker->getIntersections();
//                 osgUtil::PolytopeIntersector::Intersections::iterator iter = intersections.begin();
//                 std::cout<<">>>>>>>>>>> Begin >>>>>>>>>>>>>"<<std::endl;
//                 for (;iter !=intersections.end();iter++)
//                 {
//                     osgUtil::PolytopeIntersector::Intersection intersection1 = *iter;
// 
//                     unsigned int idx2 = intersection1.nodePath.size();
//                     std::cout<<"        NodePath size"<<dec<<noshowbase<<idx2<<std::endl;
// 
//                     while (idx2--)
//                     {
//                         osg::Node* node = intersection1.nodePath[idx2];
//                         std::cout<<"            Hits "<<dec<<noshowbase<<idx2<< ' '<<hex<<showbase<<intersection1.nodePath[idx2]->getNodeMask()<<std::endl;
//                     }
//                     
//                 }
//                 std::cout<<"<<<<<<<<<< End <<<<<<<<<<<<"<<std::endl;
//             }

			osgUtil::PolytopeIntersector::Intersection& intersection = 
				picker->getFirstIntersection();

			osg::NodePath& nodePath =
				intersection.nodePath;
			unsigned int idx = nodePath.size();
			osgManipulator::Selection* sel=0;

            bool selectsignobject = false;
            osg::Geode* signgeo = NULL;
			while (idx--)	{
				// 查找交集节点路径中的最后一个osgManipulator::Selection
                signgeo =dynamic_cast<osg::Geode*>(nodePath[ idx ]);
                if(signgeo && ( (signgeo->getNodeMask() | CLASSCODE::ClassSignObject) == CLASSCODE::ClassSignObject))
                {
                    _selectedSignObject = dynamic_cast<SignObject*>(signgeo->getParent(0));
                    selectsignobject = _selectedSignObject?true:false;
                    break;
                }

				sel =	dynamic_cast<osgManipulator::Selection*>( nodePath[ idx ] );
				if (sel == NULL)
					continue;
				
				SignObject* sign = dynamic_cast<SignObject*>(sel->getChild(0));
				if(sign==NULL)
					continue;
				
				CloudStation* cloudstation = dynamic_cast<CloudStation*>(sel);
				sign->AddOrEditSign(intersection.localIntersectionPoint, curID);
				sign->HighLightSign(curID);

				if( cloudstation && cloudstation->GetSignObjectDataCallBack()) 
				{
					(*cloudstation->GetSignObjectDataCallBack())(intersection.localIntersectionPoint, curID);
				}

				return true;
			}

            if(selectsignobject)
            {
                curID = _selectedSignObject->GetSignID(signgeo);
                _selectedSignObject->HighLightSign(signgeo);
            }
		}

		return true;
	}

	return true;
}

void LabelPointImpl::DeleteSelectedSignObject()
{
    if(_selectedSignObject)
    {
        _selectedSignObject->DeleteSign(curID);
		CloudStation* cloudstation = dynamic_cast<CloudStation*>(_selectedSignObject->getParent(0));
		if( cloudstation && cloudstation->GetSignObjectDataCallBack()) 
		{
			(*cloudstation->GetSignObjectDataCallBack())(osg::Vec3(0,0,0), curID);
		}
    }
}