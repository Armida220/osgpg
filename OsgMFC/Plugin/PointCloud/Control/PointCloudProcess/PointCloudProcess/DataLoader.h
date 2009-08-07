/********************************************************************
filename: 	Project\PointCloud\PointCloudProcess\DataLoader.h
file path:	Project\PointCloud\PointCloudProcess
file base:	DataLoader
file ext:	h

purpose:	点云数据和三角网数据装载器
*********************************************************************/
#pragma once
#include "export.h"
#include "CloudStation.h"
#include "TriangleMesh.h"

//点云文件文本型读写器
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

//三角网文件读写器
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

// TODO 二进制类型