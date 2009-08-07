#include "StdAfx.h"
#include "TriStripSimplifier.h"
#include "osgDB/readfile"
#include "osgDB/writefile"
#include "osgUtil/tristripvisitor"

TriStripSimplifier::TriStripSimplifier(void)
{
}

TriStripSimplifier::~TriStripSimplifier(void)
{
}

void TriStripSimplifier::operator()(osg::PagedLOD &node)
{
	for(unsigned int i=0; i<node.getNumFileNames(); ++i) {
		//read
		//const std::string &strDBPath = node.getDatabasePath();
		const std::string strFilePath = /*strDBPath +*/ node.getFileName(i);
		osg::ref_ptr<osg::Node> pSubNode = osgDB::readNodeFile(strFilePath);

		if(!pSubNode.valid()) return;
		osg::Geode *pGeode = pSubNode->asGeode();
		if(!pGeode)		return;

		//process
		osgUtil::TriStripVisitor stripper;
		pGeode->accept(stripper);
		stripper.stripify();

		//write
		node.setDatabasePath("");
		osg::ref_ptr<osgDB::ReaderWriter::Options> pOptRW = new osgDB::ReaderWriter::Options;
		pOptRW->setOptionString("noTexturesInIVEFile");
		osgDB::Registry::instance()->setOptions(pOptRW.get());

		osgDB::writeNodeFile(*pGeode, strFilePath);
	}
}