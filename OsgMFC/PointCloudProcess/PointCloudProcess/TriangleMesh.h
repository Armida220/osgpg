/********************************************************************
filename: 	Project\PointCloud\PointCloudProcess\TriangleMesh.h
file path:	Project\PointCloud\PointCloudProcess
file base:	TriangleMesh
file ext:	h

purpose:	ÃèÊöÈý½ÇÍøÊý¾Ý
*********************************************************************/
#pragma once
#include "CloudStation.h"
#include "export.h"

class POINTCLOUDPROCESS_API TriangleMesh : public CloudStation
{
public:
	TriangleMesh(void);

	virtual const char* className() const { return "TriangleMesh"; }

	virtual bool SetColorPerVertex(osg::Vec4Array& colors);
	virtual osg::ref_ptr<osg::Vec3Array> GetTransformedPointCloud();

	bool SetTexture(osg::Vec2Array& textureCoords, string textureName);

public:
	//return -1 means vertexArr none, -2 means triArr none
	double GetTriangleArea();

	osg::Vec3Array* GetVertexArray();
	const vector<osg::DrawElementsUInt*>* GetTriangleArray();

protected:
	//osg::ref_ptr<osg::Vec3Array> vertexArr;
	vector<osg::DrawElementsUInt*> triArr;
};
