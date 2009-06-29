#include "StdAfx.h"
#include "DataLoader.h"
#include "Points2Grids.h"
#include "CloudStation.h"
#include "../DLLFiles/DebugDLL/PoissonDLL.h"
#include "../DLLFiles/DebugDLL/TriStruct.h"
#include "plyreader.h"
#include "osgUtil/SmoothingVisitor"
#include "PointsDividor.h"


CloudStation* PointCloudReadWriter::Read(char* filename, unsigned int fileType)
{
	if(filename==0)
		return 0;

	CloudStation* ret = 0;

	if(fileType==RAW_POINT_CLOUD) {
		std::fstream file(filename);
		Points2Grids p2g(file, 10);//均匀分布，则每个格子10个点
		p2g.process();

		osg::Group* scene = p2g.getRoot().get();
		scene->setName("PointCloud");

		ret = new CloudStation;
		ret->addChild(scene);
	} else if(fileType==KDTREE_POINT_CLOUD) {//使用Kd树进行LOD
		std::fstream infile(filename);
		FC::PointsDividor pd(infile, "kdTree.log");
		pd.process();

		osg::Group* scene = pd.GetRoot().get();
		scene->setName("KdGrids_PointCloud");
		
		ret = new CloudStation;
		ret->addChild(scene);
	} else if(fileType==OSG_FILE) {
		osg::Node* scene = osgDB::readNodeFile(string(filename));
		scene->setName("PointCloud");

		ret = new CloudStation;
		ret->addChild(scene);
	}

	return ret;
}

void PointCloudReadWriter::Write(char* filename, CloudStation& cloudstation)
{
	if(filename==NULL)
		return;

	osg::Node* scene = 0;
	scene = cloudstation.getChild(1);

	if(scene) {
		osgDB::writeNodeFile(*scene, filename);
	}
}

TriangleMesh* TriMeshReadWriter::Read(char* filename, unsigned int fileType)
{
	if(filename==NULL)
		return 0;

	TriangleMesh* ret = new TriangleMesh;

	if(fileType==TriMeshReadWriter::POINT_CLOUD ||
		fileType==TriMeshReadWriter::PLY_FILE) {
		typeMesh trimesh;
		if(fileType==POINT_CLOUD)
			PoissonReconBuffer(filename,trimesh);
		else
			PlyRead(filename, trimesh);

		osg::Geode *geode = new osg::Geode;
		osg::Geometry *galaxy = new osg::Geometry;
		osg::Vec4Array *colorsArr = new osg::Vec4Array;
		osg::Vec3Array *pointsArr = new osg::Vec3Array;
		osg::IntArray* iA = new osg::IntArray;

		for(int i=0; i<trimesh.PointNum; i++) {
			typePoint3D& p = trimesh.Points[i];
			pointsArr->push_back(osg::Vec3(p.x, p.y, p.z));
		}

		galaxy->setVertexArray(pointsArr);
		galaxy->setColorArray(colorsArr);

		colorsArr->push_back(osg::Vec4(1,1,0,0));//for init color
		colorsArr->push_back(osg::Vec4(0,1,0,0));//for label triangle
		galaxy->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
		galaxy->setColorIndices(iA);

		for(int i=0; i<trimesh.TriNum; i++) {
			typeTri& tri = trimesh.Triangles[i];
			GLuint triIdx[3];
			triIdx[0] = tri.pIndex1;triIdx[1] = tri.pIndex2;triIdx[2] = tri.pIndex3;
			galaxy->addPrimitiveSet(new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES,
				3,triIdx));
			iA->push_back(0);//color idx point to the first color in the color array
		}

		//galaxy->setUseVertexBufferObjects(true);
		geode->addDrawable(galaxy);
		geode->setInitialBound( geode->getBound() );

		//osg::StateSet *set = geode->getOrCreateStateSet();
		//set->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		geode->setName("TriangleMesh");

		ret->addChild(geode);
	} else {
		osg::Node* mesh = osgDB::readNodeFile(filename);
		if(mesh)
			mesh->setName("TriangleMesh");

		ret->addChild(mesh);
	}

	osgUtil::SmoothingVisitor sv;
	ret->accept(sv);
	
	return ret;
}

void TriMeshReadWriter::Write(char* filename, TriangleMesh& triangleMesh)
{
	if(filename==NULL)
		return;

	osg::Node* mesh = 0;
	mesh = triangleMesh.getChild(1);

	if(mesh) {
		osgDB::writeNodeFile(*mesh, filename);
	}
}