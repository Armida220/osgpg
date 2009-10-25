/********************************************************************
filename: 	Project\PointCloud\PointCloudProcess\SignObject.h
file path:	Project\PointCloud\PointCloudProcess
file base:	SignObject
file ext:	h

purpose:	标记对象表示对点云中的点进行标记
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

	//在pos处添加序号为id的标记对象
	void AddOrEditSign(osg::Vec3 pos, unsigned int id);

	//高亮某一序号的标记对象，并去掉前一次高亮对象的高亮状态
	void HighLightSign(unsigned int id);

    //高亮选中的标记对象
    void HighLightSign(osg::Geode* geom);

    // 删除
    unsigned int DeleteSign(unsigned int id);

    unsigned int GetSignID(osg::Geode* geom);

protected:
	//创建场景中的节点,文本中的序号用 GetNumber()表示。
	osg::Geode* CreateNode(osg::Vec3 pos, unsigned int id);
	//编辑修改节点
	void EditNode(osg::Geode* geode, osg::Vec3 pos);

	void SetTextColor(osgText::Text* text, osg::Vec4 c);

protected:
	SignMap m_signMap;

	//当前高亮标记对象的id
	unsigned int curID;

	osg::Vec4 m_highLightColor;
	osg::Vec4 m_commonColor;
};
