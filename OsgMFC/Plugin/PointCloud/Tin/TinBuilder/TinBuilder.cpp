#include "stdafx.h"
#include "TinBuilder.h"
#include "TinBuildAPI.h"

#include <osg/ref_ptr>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Image>
#include <osg/PrimitiveSet>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Texture2D>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

osg::Geode* makeTin(FC::TinDataFile& tdf)
{
	osg::Geode* geode = new osg::Geode;

	FC::TextureID2TriangleSet::iterator itr=tdf._tex2TriSet.begin();
	FC::TextureID2TriangleSet::iterator eitr=tdf._tex2TriSet.end();
	for(; itr!=eitr; ++itr) {
		FC::TriangleSet& ts = itr->second;
		if(ts.size()<=0) {
			continue;
		}

		osg::Geometry* geom = new osg::Geometry;
		GLuint* idx = new GLuint[ts.size()*3];
		long*		flag = new long[tdf._points->size()];
		memset(flag, -1, sizeof(long)*tdf._points->size());
		{
			for(unsigned int i=0; i<ts.size(); ++i) {
				FC::TriangleRecord& tr = ts[i];
				idx[i*3  ] = tr._idx[0];
				idx[i*3+1] = tr._idx[1];
				idx[i*3+2] = tr._idx[2];

				flag[tr._idx[0]]=tr._idx[0];
				flag[tr._idx[1]]=tr._idx[1];
				flag[tr._idx[2]]=tr._idx[2];
			}

			osg::Vec3Array* v3a = new osg::Vec3Array;
			long cnt=0;
			for(unsigned int i=0; i<tdf._points->size(); ++i) {
				if(flag[i]!=-1) {
					flag[i] += cnt;
					v3a->push_back( tdf._points->at(i) );
				} else {
					cnt+=-1;
				}
			}
			geom->setVertexArray(v3a);

			osg::Vec2Array* v2a = new osg::Vec2Array(v3a->size());
			for(unsigned int i=0; i<ts.size(); ++i) {
				idx[i*3  ] = flag[ idx[i*3  ] ];
				idx[i*3+1] = flag[ idx[i*3+1] ];
				idx[i*3+2] = flag[ idx[i*3+2] ];

				FC::TriangleRecord& tr = ts[i];
				v2a->at( idx[i*3] ).set(tr._tc[0][0], tr._tc[0][1]);
				v2a->at( idx[i*3+1] ).set(tr._tc[1][0], tr._tc[1][1]);
				v2a->at( idx[i*3+2] ).set(tr._tc[2][0], tr._tc[2][1]);
			}
			geom->setTexCoordArray(0, v2a);

			geom->addPrimitiveSet(
				new osg::DrawElementsUInt(
				osg::PrimitiveSet::TRIANGLES, ts.size()*3, idx)
				);
			osg::StateSet* ss = geom->getOrCreateStateSet();
			osg::Image* image = 
				osgDB::readImageFile(tdf._texturesPath[itr->first]);
			osg::Texture2D* tex = new osg::Texture2D;
			tex->setImage(image);
			ss->setTextureAttributeAndModes(0, tex);
		}
		delete [] idx;
		delete [] flag;

		geode->addDrawable(geom);
	}
	
	return geode;
}

//////////////////////////////////////////////////////////////////////////

bool CreateWorkFlowPlugin(void **pobj)
{
	*pobj = new FC::TinBuilder;
	return *pobj != NULL;
}

namespace FC {
	void TinBuilder::Release()
	{
		delete this;
	}
	void TinBuilder::SetSceneData(osgViewer::CompositeViewer* viewer)
	{
		osg::Group* root = viewer->getView(0)->getSceneData()->asGroup();

		setlocale(LC_ALL, "chs");

		AfxMessageBox("第一步：选择Tin文件");

		CFileDialog dlg(TRUE, "选择Tin文件", NULL, 0, "ALL(*.*)|*.*|");
		if(dlg.DoModal()!=IDOK) {
			AfxMessageBox("没有选择Tin文件，工作流结束！");
			return;
		}
		CString strFileName = dlg.GetPathName();

		string tinPath(strFileName);
		std::ifstream in(tinPath.c_str());

		TinDataFile tdf;
		ReadTinHead(tdf, in);
		ReadTinPoints(tdf, in);
		ReadTinTriangles(tdf, in);
		ReadTinTexturesPath(tdf, in);

		this->sceneRoot = makeTin(tdf);

		root->addChild(sceneRoot);

		AfxMessageBox("第二步：浏览Tin模型");
	}
}