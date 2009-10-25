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

    // 选择点云的部分，实现时生成一个临时点云，针对这个选中的部分点云进行移动和删除

protected:
	osgManipulator::Selection* sel;

    // 框选对象
    Utility::SelectBox _vizselectbox;
    
    // 鼠标按下起始
    osg::ref_ptr<const osgGA::GUIEventAdapter> _eaStart;

    // 鼠标释放
    osg::ref_ptr<const osgGA::GUIEventAdapter> _eaEnd;

    // 是否处于框选状态
    bool _onvizselectbox;

    // 是否处于拖拽选中的点状态下
    bool _ondragselectedpoint;

    CloudStation* _currentCloudStation;
    
    // 对选中对象进行拖拽控制的对象，主要提供拖拽时的Event控制。
    // 使用时只需加入需要进行拖拽的对象即可。
    osg::ref_ptr<CommDragImpl> _dragControlor;

    // 选中对象后表示拖拽操作的伪对象，这里保存是伪对象在场景中
    // 的父节点，删除伪对象需要使用这个对象。
    osg::ref_ptr<osg::Group> _pseudSelectedObject;

    // 判断是否进入框选状态
    bool computeInVizSelectBox(const osgGA::GUIEventAdapter& ea,osgViewer::View& view);

    // 进行框选处理
    void processSelectBoxPick(const osgGA::GUIEventAdapter& ea,osgViewer::View& view);

    // 清除已记录的鼠标点
    void resetMouseposition();

    // 选中的顶点集合
    osg::ref_ptr<osg::Vec3Array> _selectedpoints;

    // 用索引值和Geometry表示选中的点
    GroupGeometryIndexsV _selectedpointsindexs;

    // 进入对选中点的拖拽模式，返回创建拖拽器的节点
    osg::Node* enterDragSelectedPoint(void);

    // 控制选中对象拖拽时对实际点云顶点数据的修改
    void processfeedback(void);

    // 离开选中对象拖拽的状态
    void leaveDragSelectedPoint();

    // 删除点云上选中的顶点
    void DeleteSelectedPoints();
};
