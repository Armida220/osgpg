/********************************************************************
	filename: 	Project\PointCloud\PointCloudProcess\DataCallBack.h
	file path:	Project\PointCloud\PointCloudProcess
	file base:	DataCallBack
	
	purpose:	���÷�ʹ�ø����ͻ������Ļ�������������ݣ������ѡ�ĵ�
                ������ǵ�����
*********************************************************************/
#pragma once
#include "export.h"

class POINTCLOUDPROCESS_API DataCallBack : public osg::Referenced
{
public:
    DataCallBack(void);

	//signlocation��ʾ��Ƕ����λ�ã�signID��ʾ��Ƕ�������
	//signlocationΪ���ʾɾ�����Ϊsignid�ı�Ƕ���
    virtual void operator()(osg::Vec3 signlocation,unsigned int signID){}

protected:
    ~DataCallBack(void);
};
