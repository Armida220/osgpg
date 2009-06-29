/********************************************************************
filename: 	Project\PointCloud\PointCloudProcess\DataLoader.h
file path:	Project\PointCloud\PointCloudProcess
file base:	DataLoader
file ext:	h

purpose:	�������ݺ�����������װ����
*********************************************************************/
#pragma once
#include "export.h"
#include "CloudStation.h"
#include "TriangleMesh.h"

//�����ļ��ı��Ͷ�д��
class POINTCLOUDPROCESS_API PointCloudReadWriter
{
public:
	PointCloudReadWriter(void) {}
	enum FILE_TYPE {
		RAW_POINT_CLOUD=0,
		KDTREE_POINT_CLOUD,
		OSG_FILE
	};

	void Write(char* filename, CloudStation& cloudstation);
	CloudStation* Read(char* filename, unsigned int fileType=RAW_POINT_CLOUD);
};

//�������ļ���д��
class POINTCLOUDPROCESS_API TriMeshReadWriter
{
public:
	TriMeshReadWriter(void) {}
	enum FILE_TYPE {
		POINT_CLOUD=0,
		TRIANGLE_MESH,
		PLY_FILE
	};

	void Write(char* filename, TriangleMesh& triangleMesh);
	TriangleMesh* Read(char* filename, unsigned int fileType=POINT_CLOUD);
};

// TODO ����������