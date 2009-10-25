#include "StdAfx.h"
#include "SignObject.h"
#include <osgText/Text>
#include <sstream>
#include "ObjectClassCode.h"
#include "Utility.h"
using namespace PointCloudProcess;

SignObject::SignObject()
{
	this->setName("SignObject");

	m_commonColor=osg::Vec4(1,0,1,1);
	m_highLightColor=osg::Vec4(0,1,0,1);

	curID = 0;

	this->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
    setNodeMask(CLASSCODE::ClassSignObjects);
}

//TODO ��Ƕ���ߴ����⣬Ӧ������Ļ��С�仯�������ǹ̶��ߴ�
osg::Geode* SignObject::CreateNode(osg::Vec3 pos, unsigned int id)
{
	osg::Geode* geode = new osg::Geode();

	std::string timesFont("fonts/arial.ttf");

	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	osgText::Text* text = new  osgText::Text;
	geode->addDrawable( text );

	text->setPosition(pos);
	std::stringstream ss;
	ss << id;
	text->setText(ss.str());
	text->setFont(timesFont);
	text->setColor(osg::Vec4(0, 1, 0, 1));
	/*text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);*/
    text->setCharacterSizeMode(osgText::Text::OBJECT_COORDS_WITH_MAXIMUM_SCREEN_SIZE_CAPPED_BY_FONT_HEIGHT);
    text->setBackdropType(osgText::Text::OUTLINE);
	text->setAxisAlignment(osgText::Text::SCREEN);

	// ����һ�����ڱ��漸����Ϣ�Ķ��� 
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry; 

	// �����ĸ���������� 
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array; 
	geom->setVertexArray( v.get() ); 
	v->push_back(pos);
	double x = pos.x();
	double y = pos.y();
	double z = pos.z();
	v->push_back(osg::Vec3(x-0.1,y-0.1,z));
	v->push_back(osg::Vec3(x+0.1,y-0.1,z));
	v->push_back(osg::Vec3(x+0.1,y+0.1,z));
	v->push_back(osg::Vec3(x-0.1,y+0.1,z));

	unsigned int lines[] = {
		0,1,0,2,0,3,0,4
	};
	geom->addPrimitiveSet( 
		new osg::DrawElementsUInt( osg::PrimitiveSet::LINES, 8, lines ) ); 

	// �� Geode ����Ӽ����壨Drawable�������� Geode 
	geode->addDrawable( geom.get() ); 

    geode->setName("SignObject");
    geode->setNodeMask(CLASSCODE::ClassSignObject);
	return geode;
}

//TODO ��Ƕ���ߴ����⣬Ӧ������Ļ��С�仯�������ǹ̶��ߴ�
void SignObject::EditNode(osg::Geode* geode, osg::Vec3 pos)
{
	osgText::Text* text = dynamic_cast<osgText::Text*>(
		geode->getDrawable(0));
	if(text) {
		text->setPosition(pos);
	}

	if(geode->getNumDrawables()>1) {
		osg::Geometry* geom = dynamic_cast<osg::Geometry*>(
			geode->getDrawable(1));
		if(geom) {
			osg::Vec3Array* v3a = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			if(v3a && v3a->size()==5) {
				double x = pos.x();
				double y = pos.y();
				double z = pos.z();

				osg::Vec3& v0 = v3a->at(0);
				v0.set(pos);

				osg::Vec3& v1 = v3a->at(1);
				v1.set(x-0.1,y-0.1,z);

				osg::Vec3& v2 = v3a->at(2);
				v2.set(x+0.1,y-0.1,z);

				osg::Vec3& v3 = v3a->at(3);
				v3.set(x+0.1,y+0.1,z);

				osg::Vec3& v4 = v3a->at(4);
				v4.set(x-0.1,y+0.1,z);
			}
			geom->dirtyDisplayList();
		}
	}
}

void SignObject::AddOrEditSign(osg::Vec3 pos, unsigned int id)
{
	SignMap::iterator itr = m_signMap.find(id);
	if(itr!=m_signMap.end()) { // have already, edit
		SignPair& sp = itr->second;

		osg::Geode* geode = sp.first.get();
		EditNode(geode, pos);

		osg::Vec3& oldPos = sp.second;
		oldPos.set(pos);
	} else { // have not, add
		osg::ref_ptr<osg::Geode> sign = CreateNode(pos, id);
		this->addChild(sign.get());

		SignPair sp(sign, pos);
		m_signMap.insert(SignMap::value_type(id, sp));
	}
}

void SignObject::HighLightSign(unsigned int id)
{
	if(id==curID)
		return;

	SignMap::iterator oldItr = m_signMap.find(curID);
	//ȥ����ǰ�ĸ���
	if(oldItr!=m_signMap.end()) {
		SignPair& sp = oldItr->second;

		osg::Geode* geode = sp.first.get();
		osgText::Text* text = dynamic_cast<osgText::Text*>(
			geode->getDrawable(0));
		if(text) {
			SetTextColor(text, m_commonColor);
		}
	}

	curID = id;
	SignMap::iterator itr = m_signMap.find(id);
	//����id
	if(itr!=m_signMap.end()) {
		SignPair& sp = itr->second;

		osg::Geode* geode = sp.first.get();
		osgText::Text* text = dynamic_cast<osgText::Text*>(
			geode->getDrawable(0));
		if(text) {
			SetTextColor(text, m_highLightColor);
		}
	}
}

void SignObject::HighLightSign( osg::Geode* geom )
{
    if(!geom) return;

    SignMap::iterator oldItr = m_signMap.find(curID);
    //ȥ����ǰ�ĸ���
    if(oldItr!=m_signMap.end()) {
        SignPair& sp = oldItr->second;

        osg::Geode* geode = sp.first.get();
        osgText::Text* text = dynamic_cast<osgText::Text*>(
            geode->getDrawable(0));
        if(text) {
            SetTextColor(text, m_commonColor);
        }
    }

    osgText::Text* text = dynamic_cast<osgText::Text*>(geom->getDrawable(0));
    if(text) {
        SetTextColor(text, m_highLightColor);
    }

    curID = GetSignID(geom);
}

void SignObject::SetTextColor(osgText::Text* text, osg::Vec4 c)
{
	osg::Vec4 color = text->getColor();
	text->setColor(c);
	text->dirtyDisplayList();
}

unsigned int SignObject::DeleteSign( unsigned int id )
{
    SignMap::iterator oldItr = m_signMap.find(curID);
    //ȥ����ǰ�ĸ���
    if(oldItr!=m_signMap.end()) 
    {
        SignPair& sp = oldItr->second;

        osg::Geode* geode = sp.first.get();
        Utility::DeleteSelectedNodesVisitor dsn(CLASSCODE::ClassSignObject);
        geode->accept(dsn);
        dsn.pruneSelectedNodes();
        m_signMap.erase(oldItr);
    }

    return (unsigned int)m_signMap.size();
}

unsigned int SignObject::GetSignID( osg::Geode* geom )
{
    unsigned int retID = 0xFFFFFFFF;
    
    if(!geom) return retID;

    SignMap::iterator iter = m_signMap.begin();
    for (;iter != m_signMap.end();iter++)
    {
        SignPair& sp = iter->second;
        if(geom == sp.first.get())
        {
            retID = iter->first;
            break;
        }
    }

    return retID;
}