#include "StdAfx.h"

#include <osg/Group>
#include <osgViewer/Viewer>
#include <osg/NodeVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include "TriangleSimplifier.h"


TriangleSimplifier::TriangleSimplifier(void)
{
	m_setLod.clear();
	m_dblAreaThreshold = 10.0;
	m_dblAngleThreshold = 0.2;
}


TriangleSimplifier::~TriangleSimplifier(void)
{
}


void TriangleSimplifier::SetThreshold(double dblArea, double dblAngle)
{
	m_dblAreaThreshold = dblArea;

	if(dblAngle > 0.5)		dblAngle = 0.5;
	else if(dblAngle < 0.0)	dblAngle = 0.0;
	m_dblAngleThreshold = dblAngle;
}


bool TriangleSimplifier::NeedSimplify(osg::PagedLOD &node)
{
	std::set<osg::PagedLOD *>::iterator itorFind = m_setLod.find(&node);
	if(itorFind == m_setLod.end())
	{
		m_setLod.insert(&node);
		if(node.getRangeList().size() > 1)
		{
			return true;
		}
	}
	return false;
}


void TriangleSimplifier::operator()(osg::PagedLOD &node)
{
	if(!NeedSimplify(node))
	{
		return;
	}

	/*const std::string &strDBPath = node.getDatabasePath();*/
	const std::string strFilePath = /*strDBPath + */node.getFileName(0);
	osg::ref_ptr<osg::Node> pSubNode = osgDB::readNodeFile(strFilePath);

	if(!pSubNode.valid()) return;//FC
	osg::Geode *pGeode = pSubNode->asGeode();
	if(!pGeode)		return;

	for(unsigned int i = 0; i < pGeode->getNumDrawables(); i++)
	{
		osg::Geometry *pGeom = pGeode->getDrawable(i)->asGeometry();

		bool canSimplify=true;
		for(unsigned int j = 0; j < pGeom->getNumPrimitiveSets(); j++)
		{
			osg::PrimitiveSet *pPriSet = pGeom->getPrimitiveSet(j);

			osg::DrawArrays *pDA = dynamic_cast<osg::DrawArrays*>(pPriSet);
			if(pDA==NULL)
			{
				canSimplify = false;
				break;
			}

			if(pPriSet->getMode() != osg::PrimitiveSet::TRIANGLES)
			{
				//目前只处理TRIANGLES,若有一个不是Triangles则跳过不处理
				canSimplify = false;
				break;
			}
		}
		//因为所有primitives共用同一个顶点数组，若有一个primitive不是Triangles则跳过不处理
		if(!canSimplify)
			continue;

		for(unsigned int j = 0; j < pGeom->getNumPrimitiveSets(); j++)
		{
			osg::PrimitiveSet *pPriSet = pGeom->getPrimitiveSet(j);

			osg::Vec3Array* v3a  = dynamic_cast<osg::Vec3Array*>(pGeom->getVertexArray());
			osg::Vec2Array* v2an = dynamic_cast<osg::Vec2Array*>(pGeom->getNormalArray());

			std::vector<osg::Vec2Array*> v2ats;
			v2ats.clear();
			for(unsigned int ti = 0; ti < pGeom->getNumTexCoordArrays(); ti++)
			{
				v2ats.push_back(dynamic_cast<osg::Vec2Array*>(pGeom->getTexCoordArray(ti)));
			}

			unsigned int nk = 0, tk = 0;
			for(unsigned int k = 0; k < v3a->size(); k += 3, nk += 3, tk += 3)
			{
				osg::Vec3& v1 = v3a->at(k);
				osg::Vec3& v2 = v3a->at(k + 1);
				osg::Vec3& v3 = v3a->at(k + 2);

				if(area(v1, v2, v3) < m_dblAreaThreshold || 
					minAngle(v1, v2, v3) < m_dblAngleThreshold)	//面积过小，或是过于细长舍弃
				{
					v3a->erase(v3a->begin() + k + 2);
					v3a->erase(v3a->begin() + k + 1);
					v3a->erase(v3a->begin() + k);
					k-=3;

					if(v2an)
					{
						v2an->erase(v2an->begin() + nk + 2);
						v2an->erase(v2an->begin() + nk + 1);
						v2an->erase(v2an->begin() + nk);
						nk-=3;
					}

					for(unsigned int ti = 0; ti < pGeom->getNumTexCoordArrays(); ti++)
					{
						osg::Vec2Array *v2at = v2ats[ti];
						if(v2at)
						{
							v2at->erase(v2at->begin() + tk + 2);
							v2at->erase(v2at->begin() + tk + 1);
							v2at->erase(v2at->begin() + tk);
						}
					}
					tk-=3;
				}//end of if
			}//end of index

			if(v3a->size() != 0)
			{
				pGeom->setPrimitiveSet(j, new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, v3a->size()));
			}
			else
			{
				pGeom->removePrimitiveSet(j);
				--j;
			}
		}//end of primitiveset
	}//end of drawable

	node.setDatabasePath("");
	osg::ref_ptr<osgDB::ReaderWriter::Options> pOptRW = new osgDB::ReaderWriter::Options;
	pOptRW->setOptionString("noTexturesInIVEFile");
	osgDB::Registry::instance()->setOptions(pOptRW.get());

	osgDB::writeNodeFile(*pGeode, strFilePath);
}

