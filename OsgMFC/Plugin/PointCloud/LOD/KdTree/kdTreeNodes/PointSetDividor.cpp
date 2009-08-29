#include "PointSetDividor.h"

#include <osgDB//FileNameUtils>
#include <osgDB//WriteFile>
#include <osg/PagedLOD>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "RawPointReader.h"
#include "PointLODGenerator.h"

using namespace std;
using namespace FC;

bool PointSetDividor::Process(BuildParameters bp /* = BuildParameters */)
{
	RawPointReader reader;
	PointLODGenerator generator;
	unsigned int recordType = XYZ;

	string saveFilePathNoName = osgDB::getFilePath(_saveFileName);
	if(saveFilePathNoName[saveFilePathNoName.length()-1]!='\\' 
		|| saveFilePathNoName[saveFilePathNoName.length()-1]!='/')
		saveFilePathNoName+="\\";
	string saveFileOnlyName = osgDB::getSimpleFileName(_saveFileName);

	//init
	bool initRet = reader.Init(_dataFileName.c_str(), recordType);
	bp._recordType = recordType;
	if(!initRet) {
		_log<<"Read Init Error!"<<endl;
		return initRet;
	} else {
		_log<<"Read Init Complete!"<<endl;
	}

	//read and generate
	_root = new osg::Group;
	int pagedLodNum = 0;
	while(reader.Read(_pointSet)) {
		osg::ref_ptr<osg::LOD> lodRoot = new osg::LOD;
		bool genRet = generator.Generate(_pointSet, lodRoot, bp);

		if(!genRet) {
			_log<<"Generating Error!"<<endl;
			return genRet;
		} else {
			_log<<"Generating..."<<endl;
		}

		char lodSaveName[2048];
		sprintf(lodSaveName, "%d%s\0", pagedLodNum++, saveFileOnlyName.c_str());
		string saveLodFullName = saveFilePathNoName;
		saveLodFullName += string(lodSaveName);
		osgDB::writeNodeFile(*lodRoot, saveLodFullName.c_str());
		_log <<"Writing pagedLod File : "<<saveLodFullName<<endl;

		osg::ref_ptr<osg::PagedLOD> plod = new osg::PagedLOD;
		plod->setRangeMode(osg::LOD::PIXEL_SIZE_ON_SCREEN);
		plod->setCenter(lodRoot->getCenter());
		plod->setRadius(lodRoot->getRadius());
		plod->setFileName(0, string(lodSaveName));
		plod->setRange(0, bp._maxBoundVisibleRange, FLT_MAX);
		_root->addChild(plod.get());

		_pointSet.clear();//ATTENTION clear the point set
	}
	_log<<"Generate Complete!"<<endl;

	//close
	reader.Close();

	return true;
}