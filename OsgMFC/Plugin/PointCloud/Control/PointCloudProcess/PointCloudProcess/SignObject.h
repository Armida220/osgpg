/********************************************************************
filename: 	Project\PointCloud\PointCloudProcess\SignObject.h
file path:	Project\PointCloud\PointCloudProcess
file base:	SignObject
file ext:	h

purpose:	��Ƕ����ʾ�Ե����еĵ���б��
*********************************************************************/
#pragma once
#include "export.h"
#include <osgText/Text>
#include <map>
using namespace std;

class POINTCLOUDPROCESS_API SignObject : public osg::Group
{
public:
	SignObject();

	virtual const char* className() const { return "SignObject"; }

public:
	typedef std::pair< osg::ref_ptr<osg::Geode>, osg::Vec3 > SignPair;
	typedef map<unsigned int, SignPair> SignMap;

	void SetHighLightColor(osg::Vec4& color) { m_highLightColor=color; }
	osg::Vec4 GetHighLightColor() const { return m_highLightColor; }

	void SetCommonColor(osg::Vec4& color) { m_commonColor=color; }
	osg::Vec4 GetCommonColor() const { return m_commonColor; }
 
	int GetNumOfSigns() { return m_signMap.size(); }
	const SignMap& GetSigns() const { return m_signMap; }

	//��pos��������Ϊid�ı�Ƕ���
	void AddOrEditSign(osg::Vec3 pos, unsigned int id);

	//����ĳһ��ŵı�Ƕ��󣬲�ȥ��ǰһ�θ�������ĸ���״̬
	void HighLightSign(unsigned int id);

    //����ѡ�еı�Ƕ���
    void HighLightSign(osg::Geode* geom);

    // ɾ��
    unsigned int DeleteSign(unsigned int id);

    unsigned int GetSignID(osg::Geode* geom);

protected:
	//���������еĽڵ�,�ı��е������ GetNumber()��ʾ��
	osg::Geode* CreateNode(osg::Vec3 pos, unsigned int id);
	//�༭�޸Ľڵ�
	void EditNode(osg::Geode* geode, osg::Vec3 pos);

	void SetTextColor(osgText::Text* text, osg::Vec4 c);

protected:
	SignMap m_signMap;

	//��ǰ������Ƕ����id
	unsigned int curID;

	osg::Vec4 m_highLightColor;
	osg::Vec4 m_commonColor;
};
