#pragma once
#include "controller.h"
#include "TriangleMesh.h"
#include "DragImpl.h"

class PickTriangleImpl :	public ControlImpl
{
public:
	PickTriangleImpl(ostream& _log=cout);

	const Triangle& GetTriangle() const { return triangle; }

	bool operator()(const osgGA::GUIEventAdapter& ea, 
		osgGA::GUIActionAdapter& aa);

protected:
	//for select triangle
	Triangle triangle;
};

//////////////////////////////////////////////////////////////////////////
class PickTriangleEdgeImpl :	public ControlImpl
{
public:
    PickTriangleEdgeImpl(ostream& _log=cout);

    virtual bool operator()(const osgGA::GUIEventAdapter& ea, 
        osgGA::GUIActionAdapter& aa);

protected:

    virtual void processfeedback(void);

    void cancelSelectedEdgeDrag();

    void saveSelectedEdgeDragState();

    virtual osg::Node* enterDragSelected(void);

    TriangleMesh* _triMesh;

    osg::Vec3* p0;
    osg::Vec3* p1;
    osg::Vec3 orgp0,orgp1;
    osg::Geometry* _geom;

    // �Ƿ�����קѡ�еĵ�״̬��
    bool _ondragselected;

    // ��ѡ�ж��������ק���ƵĶ�����Ҫ�ṩ��קʱ��Event���ơ�
    // ʹ��ʱֻ�������Ҫ������ק�Ķ��󼴿ɡ�
    osg::ref_ptr<CommDragImpl> _dragControlor;
};

//////////////////////////////////////////////////////////////////////////
class PickTriangleFaceImpl :public PickTriangleEdgeImpl
{
public:
    PickTriangleFaceImpl(ostream& _log=cout);

    virtual bool operator()(const osgGA::GUIEventAdapter& ea, 
        osgGA::GUIActionAdapter& aa);

protected:

    virtual void processfeedback(void);

    void cancelSelectedFaceDrag();

    void saveSelectedFaceDragState();

    virtual osg::Node* enterDragSelected(void);

    osg::Vec3* p2;
    osg::Vec3 orgp2;
};