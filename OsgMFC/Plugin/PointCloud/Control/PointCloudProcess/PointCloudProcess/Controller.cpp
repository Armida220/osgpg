#include "StdAfx.h"
#include "Controller.h"
#include "LabelPointImpl.h"
#include "PickPointCloudImpl.h"
#include "PickTriangleImpl.h"
#include "DragVertexImpl.h"
#include "DragImpl.h"

Controller::Controller(ostream& _log) : log(_log)
{
	this->_mode=VIEW;

	_ctrlImplList.push_back(new LabelPointImpl(_log));
	_ctrlImplList.push_back(new PickPointCloudImpl(_log));
	_ctrlImplList.push_back(new PickTriangleImpl(_log));
	_ctrlImplList.push_back(new DragVertexImpl(_log));
	_ctrlImplList.push_back(new DragImpl(_log));
}

bool Controller::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	bool ret;
	ControlImpl* ci;

	switch(this->_mode)
	{
	case VIEW:
		return __super::handle(ea, aa);
	case LABEL_POINT:
		ci = _ctrlImplList[LABEL_POINT-1];
		return (*ci)(ea, aa);
	case PICK_POINT_CLOUD:
		ci = _ctrlImplList[PICK_POINT_CLOUD-1];
		return (*ci)(ea, aa);
	case PICK_TRIANGLE:
		ci = _ctrlImplList[PICK_TRIANGLE-1];
		return (*ci)(ea, aa);
	case DRAG_VERTEX:
		ci = _ctrlImplList[DRAG_VERTEX-1];
		ret=(*ci)(ea, aa);
		if(ret==false)
			return __super::handle(ea, aa);
		else
			return ret;
	case DRAG:
		ci = _ctrlImplList[DRAG-1];
		ret=(*ci)(ea, aa);
		if(ret==false)
			return __super::handle(ea, aa);
		else
			return ret;
	}

	return __super::handle(ea, aa);
}

//////////////////////////////////////////////////////////////////////////
//				invoke	Label Impl
//////////////////////////////////////////////////////////////////////////
unsigned int Controller::GetCurID()
{
	LabelPointImpl* Impl = dynamic_cast<LabelPointImpl*>(_ctrlImplList[LABEL_POINT-1].get());
	return Impl->GetCurID();
}

void Controller::SetCurID(unsigned int id)
{
	LabelPointImpl* Impl = dynamic_cast<LabelPointImpl*>(_ctrlImplList[LABEL_POINT-1].get());
	log<<"Set Current Label ID = "<<id<<endl;
	Impl->SetCurID(id);
	log<<"########################"<<endl<<endl;
}
//////////////////////////////////////////////////////////////////////////
//				invoke	Pick Point Cloud Impl
//////////////////////////////////////////////////////////////////////////
bool Controller::SetColorPerVertex(osg::Vec4Array& colors)
{
	PickPointCloudImpl* Impl = dynamic_cast<PickPointCloudImpl*>(_ctrlImplList[PICK_POINT_CLOUD-1].get());
	log<<"Set Color Per Vertex"<<endl;
	return Impl->SetColorPerVertex(colors);
	log<<"########################"<<endl<<endl;
}

osg::ref_ptr<osg::Vec3Array> Controller::GetTransformedPoints()
{
	PickPointCloudImpl* Impl = dynamic_cast<PickPointCloudImpl*>(_ctrlImplList[PICK_POINT_CLOUD-1].get());
	log<<"Get Transformed Points"<<endl;
	return Impl->GetTransformedPoints();
	log<<"########################"<<endl<<endl;
}

double Controller::GetTriangleArea()
{
	PickPointCloudImpl* Impl = dynamic_cast<PickPointCloudImpl*>(_ctrlImplList[PICK_POINT_CLOUD-1].get());
	log<<"Get Triangle Area"<<endl;
	return Impl->GetTriangleArea();
	log<<"########################"<<endl<<endl;
}

osg::Vec3Array* Controller::GetVertexArray()
{
	PickPointCloudImpl* Impl = dynamic_cast<PickPointCloudImpl*>(_ctrlImplList[PICK_POINT_CLOUD-1].get());
	log<<"Get Non-transformed Vertex Array"<<endl;
	return Impl->GetVertexArray();
	log<<"########################"<<endl<<endl;
}

bool Controller::SetTexture(osg::Vec2Array& textureCoords, string textureName)
{
	PickPointCloudImpl* Impl = dynamic_cast<PickPointCloudImpl*>(_ctrlImplList[PICK_POINT_CLOUD-1].get());
	log<<"Set Texture = "<<textureName<<endl;
	return Impl->SetTexture(textureCoords, textureName);
	log<<"########################"<<endl<<endl;
}

const vector<osg::DrawElementsUInt*>* Controller::GetTriangleArray()
{
	PickPointCloudImpl* Impl = dynamic_cast<PickPointCloudImpl*>(_ctrlImplList[PICK_POINT_CLOUD-1].get());
	log<<"Get Triangle Array"<<endl;
	return Impl->GetTriangleArray();
	log<<"########################"<<endl<<endl;
}
//////////////////////////////////////////////////////////////////////////
//				invoke	Pick Triangle Impl
//////////////////////////////////////////////////////////////////////////
const Triangle& Controller::GetTriangle()
{
	PickTriangleImpl* Impl = dynamic_cast<PickTriangleImpl*>(_ctrlImplList[PICK_TRIANGLE-1].get());
	log<<"Get Picked Triangle Information"<<endl;
	return Impl->GetTriangle();
	log<<"########################"<<endl<<endl;
}
//////////////////////////////////////////////////////////////////////////
//				invoke	Drag Impl
//////////////////////////////////////////////////////////////////////////
unsigned int Controller::GetDragMode()
{
	DragImpl* Impl = dynamic_cast<DragImpl*>(_ctrlImplList[DRAG-1].get());
	return Impl->GetDragMode();
}
void Controller::SetDragMode(unsigned int m)
{
	DragImpl* Impl = dynamic_cast<DragImpl*>(_ctrlImplList[DRAG-1].get());
	log<<"Set Drag Mode : "<<(m==Controller::BEGIN?"BEGIN":"FINISH")<<endl;
	Impl->SetDragMode(m);
	log<<"########################"<<endl<<endl;
}
void Controller::undo()
{
	DragImpl* Impl = dynamic_cast<DragImpl*>(_ctrlImplList[DRAG-1].get());
	if(Impl->undo()) {
		log<<"Drag Undo"<<endl;
		log<<"########################"<<endl<<endl;
	}
}
void Controller::redo()
{
	DragImpl* Impl = dynamic_cast<DragImpl*>(_ctrlImplList[DRAG-1].get());
	if(Impl->redo()) {
		log<<"Drag Redo"<<endl;
		log<<"########################"<<endl<<endl;
	}
}
//////////////////////////////////////////////////////////////////////////
//				invoke	Drag Vertex Impl
//////////////////////////////////////////////////////////////////////////
unsigned int Controller::GetDragVertexMode()
{
	DragVertexImpl* Impl = dynamic_cast<DragVertexImpl*>(_ctrlImplList[DRAG_VERTEX-1].get());
	return Impl->GetDragVertexMode();
}
void Controller::SetDragVertexMode(unsigned int m)
{
	DragVertexImpl* Impl = dynamic_cast<DragVertexImpl*>(_ctrlImplList[DRAG_VERTEX-1].get());
	log<<"Set Drag Vertex Mode : "<<(m==Controller::BEGIN?"BEGIN":"FINISH")<<endl;
	Impl->SetDragVertexMode(m);
	log<<"########################"<<endl<<endl;
}