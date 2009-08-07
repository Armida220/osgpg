#include "StdAfx.h"
#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(void)
{
	triArr.clear();
}

osg::Vec3Array* TriangleMesh::GetVertexArray()
{
	osg::Geode* geode = dynamic_cast<osg::Geode*>(
		this->getChild(1) );
	if(geode==0)
		return 0;

	osg::Geometry* geom = geode->getDrawable(0)->asGeometry();
	if(geom==0)
		return 0;

	return dynamic_cast<osg::Vec3Array*>( geom->getVertexArray() );
}

const vector<osg::DrawElementsUInt*>* TriangleMesh::GetTriangleArray()
{
	osg::Geode* geode = dynamic_cast<osg::Geode*>(
		this->getChild(1) );
	if(geode==0)
		return 0;

	osg::Geometry* geom = geode->getDrawable(0)->asGeometry();
	if(geom==0)
		return 0;

	this->triArr.clear();
	
	for(unsigned int i=0; i<geom->getNumPrimitiveSets(); i++) {
		osg::DrawElementsUInt* drawIdx = dynamic_cast<osg::DrawElementsUInt*>(
			geom->getPrimitiveSet( i ) );
		if(drawIdx && drawIdx->size()==3)
			this->triArr.push_back(drawIdx);
	}

	return &triArr;
}

double TriangleMesh::GetTriangleArea()
{
	osg::Vec3Array* vA=this->GetVertexArray();
	if(vA==0)
		return -1;

	const vector<osg::DrawElementsUInt*>* tA=this->GetTriangleArray();
	if(tA==0)
		return -2;

	double ret=0;
	for(unsigned int i=0; i<tA->size(); ++i) {
		const osg::DrawElementsUInt* drawIdx = (*tA)[i];

		osg::Vec3 p0 = vA->at(drawIdx->at(0));
		osg::Vec3 p1 = vA->at(drawIdx->at(1));
		osg::Vec3 p2 = vA->at(drawIdx->at(2));

		ret += ((p0-p1)^(p0-p2)).length() / 2;
	}

	return ret;
}


bool TriangleMesh::SetColorPerVertex(osg::Vec4Array& colors)
{
	osg::Geode* geode=dynamic_cast<osg::Geode*>(
		this->getChild(1));

	if(geode==NULL)
		return false;

	geode->getDrawable(0)->asGeometry()->setColorArray(&colors);
	geode->getDrawable(0)->asGeometry()->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::StateSet *set = geode->getOrCreateStateSet();
	set->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	return true;
}

osg::ref_ptr<osg::Vec3Array> TriangleMesh::GetTransformedPointCloud()
{
	osg::ref_ptr<osg::Vec3Array> ret = 0;

	osg::Geode* gd=0;
	for(unsigned int i=0; i<this->getNumChildren(); i++) {
		osg::Node* node = this->getChild(i);
		if(strcmp(node->getName().c_str(), "TriangleMesh")==0) {
			gd=dynamic_cast<osg::Geode*>(node);
			if(gd)
				break;
		}
	}

	if(gd==0)
		return ret;

	ret = new osg::Vec3Array;

	osg::Matrix mat = this->getMatrix();

	osg::Array* nv = gd->getDrawable(0)->asGeometry()->getVertexArray();
	osg::Vec3Array* v3a = dynamic_cast<osg::Vec3Array*>(nv);
	if(v3a)
		for(osg::Vec3Array::iterator itr=v3a->begin(); itr!=v3a->end(); ++itr) {
			osg::Vec3 v3 = (*itr) * mat;
			ret->push_back(v3);
		}

	return ret;
}

bool TriangleMesh::SetTexture(osg::Vec2Array& textureCoords, string textureName)
{
	osg::Geode* geode = this->getChild(1)->asGeode();
	if(geode==0)
		return false;

	osg::Geometry* geom = geode->getDrawable(0)->asGeometry();
	if(geom==0)
		return false;

	osg::StateSet* ss = geode->getOrCreateStateSet();
	osg::ref_ptr<osg::Image> image = new osg::Image;
	image->setFileName(textureName);//使用setfilename只在模型建立阶段有用，在这里osg就不会自动去打开了
	image = osgDB::readImageFile(textureName);
	if (image==NULL)	{
		return false;
	}
	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D; 
	tex->setImage( image.get() );
	ss->setTextureAttributeAndModes(0, tex.get());
	//tex->setResizeNonPowerOfTwoHint(false);

	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array; 
	geom->setColorArray( c.get() ); 
	geom->setColorBinding( osg::Geometry::BIND_OVERALL ); 
	c->push_back( osg::Vec4(1,1,1,0) );

	geom->setTexCoordArray(0, &textureCoords);

	//osgDB::writeNodeFile(*this->selNode, "D:\\test.ive");
	//osgDB::writeNodeFile(*this->sel, "D:\\test.osg");

	return true;
}