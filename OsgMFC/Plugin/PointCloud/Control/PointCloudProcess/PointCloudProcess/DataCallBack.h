/********************************************************************
	filename: 	Project\PointCloud\PointCloudProcess\DataCallBack.h
	file path:	Project\PointCloud\PointCloudProcess
	file base:	DataCallBack
	
	purpose:	调用方使用该类型获得与屏幕交互产生的数据，例如框选的点
                或做标记的数据
*********************************************************************/
#pragma once
#include "export.h"

class POINTCLOUDPROCESS_API DataCallBack : public osg::Referenced
{
public:
    DataCallBack(void);

	//signlocation表示标记对象的位置，signID表示标记对象的序号
	//signlocation为零表示删除序号为signid的标记对象
    virtual void operator()(osg::Vec3 signlocation,unsigned int signID){}

protected:
    ~DataCallBack(void);
};
