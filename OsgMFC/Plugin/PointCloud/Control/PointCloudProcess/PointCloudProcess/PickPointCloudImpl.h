#pragma once
#include "controller.h"
#include "Utility.h"
#include "CloudStation.h"
#include "DragImpl.h"

class PickPointCloudImpl :	public ControlImpl
{
public:
	PickPointCloudImpl(ostream& _log=cout);

	bool SetColorPerVertex(osg::Vec4Array& colors);
	osg::ref_ptr<osg::Vec3Array> GetTransformedPoints();

	double GetTriangleArea();

	osg::Vec3Array* GetVertexArray();//not transformed
	const vector<osg::DrawElementsUInt*>* GetTriangleArray();

	osg::ref_ptr<osg::Vec3Array> GetSignedPoints();

	bool SetTexture(osg::Vec2Array& textureCoords, string textureName);

	bool operator()(const osgGA::GUIEventAdapter& ea, 
		osgGA::GUIActionAdapter& aa);

    // ѡ����ƵĲ��֣�ʵ��ʱ����һ����ʱ���ƣ�������ѡ�еĲ��ֵ��ƽ����ƶ���ɾ��

protected:
	osgManipulator::Selection* sel;

    // ��ѡ����
    Utility::SelectBox _vizselectbox;
    
    // ��갴����ʼ
    osg::ref_ptr<const osgGA::GUIEventAdapter> _eaStart;

    // ����ͷ�
    osg::ref_ptr<const osgGA::GUIEventAdapter> _eaEnd;

    // �Ƿ��ڿ�ѡ״̬
    bool _onvizselectbox;

    // �Ƿ�����קѡ�еĵ�״̬��
    bool _ondragselectedpoint;

    CloudStation* _currentCloudStation;
    
    // ��ѡ�ж��������ק���ƵĶ�����Ҫ�ṩ��קʱ��Event���ơ�
    // ʹ��ʱֻ�������Ҫ������ק�Ķ��󼴿ɡ�
    osg::ref_ptr<CommDragImpl> _dragControlor;

    // ѡ�ж�����ʾ��ק������α�������ﱣ����α�����ڳ�����
    // �ĸ��ڵ㣬ɾ��α������Ҫʹ���������
    osg::ref_ptr<osg::Group> _pseudSelectedObject;

    // �ж��Ƿ�����ѡ״̬
    bool computeInVizSelectBox(const osgGA::GUIEventAdapter& ea,osgViewer::View& view);

    // ���п�ѡ����
    void processSelectBoxPick(const osgGA::GUIEventAdapter& ea,osgViewer::View& view);

    // ����Ѽ�¼������
    void resetMouseposition();

    // ѡ�еĶ��㼯��
    osg::ref_ptr<osg::Vec3Array> _selectedpoints;

    // ������ֵ��Geometry��ʾѡ�еĵ�
    GroupGeometryIndexsV _selectedpointsindexs;

    // �����ѡ�е����קģʽ�����ش�����ק���Ľڵ�
    osg::Node* enterDragSelectedPoint(void);

    // ����ѡ�ж�����קʱ��ʵ�ʵ��ƶ������ݵ��޸�
    void processfeedback(void);

    // �뿪ѡ�ж�����ק��״̬
    void leaveDragSelectedPoint();

    // ɾ��������ѡ�еĶ���
    void DeleteSelectedPoints();
};
