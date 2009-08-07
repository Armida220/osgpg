#include "RawPointReader.h"
#include "RawPointReadStrategy.h"
#include "PointSetDividor.h"
#include <fstream>
#include <string>
using namespace std;

using namespace FC;

RawPointReader::RawPointReader(void)
{
	_readStrategy = 0;
}

RawPointReader::~RawPointReader(void)
{
	if(_readStrategy)
		delete _readStrategy;
}

bool RawPointReader::Init(const char* fileName, unsigned int &recordType)
{
	std::ifstream inFile(fileName);
	if(!inFile) {
		cout<<"Can not open file :" <<fileName<<endl;
		return false;
	}
	unsigned int pointNum = 0;
	inFile >> pointNum;

	double x,y,z;
	double I;
	double r,g,b;
	int flag=0;

	//try
	string str;
	I = r = g = b = -1;
	getline(inFile, str, '\n');//read the first line of num of point
	getline(inFile, str, '\n');
	sscanf(str.c_str(), "%lf %lf %lf %lf %lf %lf %lf", &x, &y, &z, &I, &r, &g, &b);

	if(I==-1) {
		flag = XYZ; // x y z
	} else {
		if(r==-1) {
			flag = XYZI; // x y z i
		} else {
			if(b==-1) {
				flag = XYZRGB; // x y z r g b
			} else {
				flag = XYZIRGB; // x y z i r g b
			}
		}
	}

	recordType = flag;

	inFile.close();

	if(_readStrategy)
		delete _readStrategy;

	//点数超过阈值则使用内存映射模式
	if(pointNum<MEM_MAP_THRESHOLD)
		_readStrategy = new SimpleReadStrategy;
	else
		_readStrategy = new MemMapStrategy;

	return _readStrategy->Init(fileName, recordType);
}

bool RawPointReader::Read(PointSet &pointSet)
{
	return _readStrategy->Read(pointSet);
}

void RawPointReader::Close()
{
	_readStrategy->Close();
}