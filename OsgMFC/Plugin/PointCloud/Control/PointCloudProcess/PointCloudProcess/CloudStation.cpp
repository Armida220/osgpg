#include "stdAfx.h"
#include "CloudStation.h"
#include <osg/matrix>
#include <osg/matrixtransform>
#include "ObjectClassCode.h"
using namespace PointCloudProcess;

//////////////////////////////////////////////////////////////////////////
CloudStation::CloudStation(void)
{
	//this->m_pointscontainer3d->setUserData(this);

	//so signObj is the first child of m_pointSelection
	this->m_signObj = new SignObject;
	this->addChild(m_signObj.get());
    m_selected = false;
    m_selectedcolor = osg::Vec4(0,1,1,1);
    m_color = osg::Vec4(1,1,0,1);
	//this->setUserData(this->m_signObj.get());
    setNodeMask(CLASSCODE::ClassCloudStation);
}

osg::ref_ptr<osg::Vec3Array> CloudStation::GetTransformedPointCloud()
{
	osg::ref_ptr<osg::Vec3Array> ret = 0;

	osg::Group* pc=0;
	for(unsigned int i=0; i<this->getNumChildren(); i++) {
		osg::Node* node = this->getChild(i);
		if(strcmp(node->getName().c_str(), "PointCloud")==0) {
			pc=dynamic_cast<osg::Group*>(node);
			if(pc)
				break;
		}
	}

	if(pc==0)
		return ret;

	ret = new osg::Vec3Array;

	osg::Matrix mat = this->getMatrix();

	for(unsigned int i=0; i<pc->getNumChildren(); i++) {
		osg::Geode* geode = dynamic_cast<osg::Geode*>(pc->getChild(i));
		if(geode==NULL)
			continue;

		osg::Array* nv = geode->getDrawable(0)->asGeometry()->getVertexArray();
		osg::Vec3Array* v3a = dynamic_cast<osg::Vec3Array*>(nv);
		if(v3a==NULL)
			continue;

		for(osg::Vec3Array::iterator itr=v3a->begin(); itr!=v3a->end(); ++itr) {
			osg::Vec3 v3 = (*itr) * mat;
			ret->push_back(v3);
		}
	}

	return ret;
}

osg::ref_ptr<osg::Vec3Array> CloudStation::GetSignedPoints()
{
	osg::Matrix mat = this->getMatrix();

	osg::ref_ptr<osg::Vec3Array> ret = new osg::Vec3Array;
	const SignObject::SignMap& signMap = m_signObj->GetSigns();

	SignObject::SignMap::const_iterator itr = signMap.begin();
	for(;	itr!=signMap.end(); itr++) {
		const SignObject::SignPair& signPair = itr->second;
		osg::Vec3 v3 = signPair.second * mat;
		ret->push_back(v3);
	}

	return ret;
}

void CloudStation::SetColorToCloudStation()
{
	RemapColorVisitor rcv(this->m_color);

	this->accept(rcv);
}

bool CloudStation::SetColorPerVertex(osg::Vec4Array& colors)
{
	osg::Group* pc=dynamic_cast<osg::Group*>(
		this->getChild(1));

	if(pc==NULL)
		return false;

	osg::Vec4Array::iterator sitr=colors.begin();
	osg::Vec4Array::iterator eitr=colors.begin();
	for(unsigned int i=0; i<pc->getNumChildren(); i++) {
		osg::Geode* geode = dynamic_cast<osg::Geode*>(pc->getChild(i));
		if(geode==NULL)
			continue;

		osg::Array* nv = geode->getDrawable(0)->asGeometry()->getVertexArray();
		osg::Vec3Array* v3a = dynamic_cast<osg::Vec3Array*>(nv);
		if(v3a==NULL)
			continue;

		eitr+=v3a->size()-1;
		osg::Vec4Array* color_sub = new osg::Vec4Array(sitr, eitr);
		eitr++;
		sitr=eitr;
		geode->getDrawable(0)->asGeometry()->setColorArray(color_sub);
		geode->getDrawable(0)->asGeometry()->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		if(eitr==colors.end())
			break;
	}

	return true;
}

void CloudStation::toggleSelect()
{
    m_selected = !m_selected;

    osg::Vec4 color;

    if(m_selected)
        color = m_selectedcolor;
    else
        color = m_color;
    
    RemapColorVisitor rcv(color);
    this->accept(rcv);
}
