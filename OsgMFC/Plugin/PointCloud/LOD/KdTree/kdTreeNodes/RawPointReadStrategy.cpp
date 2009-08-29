#include "RawPointReadStrategy.h"

#include "PointSetDividor.h"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

using namespace FC;

//////////////////////////////////////////////////////////////////////////
//								Global
//////////////////////////////////////////////////////////////////////////
//Inputor concept: 
//	void getLine(string&, char)

template<class Inputor>
void ReadConcept(Inputor& in, PointSet &pointSet, 
								 unsigned int recordType)
{
	string str;
	pointSet.reserve(in._num);
	double x,y,z;
	double I;
	double r,g,b;

	PointRec::Reset_I();
	do
	{
		switch(recordType){
	case XYZ:
		in.getLine(str, '\n');
		sscanf(str.c_str(), "%lf %lf %lf", &x, &y, &z);
		pointSet.push_back(PointRec(x,y,z));
		break;
	case XYZI:
		in.getLine(str, '\n');
		sscanf(str.c_str(), "%lf %lf %lf %lf", &x, &y, &z, &I);
		pointSet.push_back(PointRec(x,y,z,I));
		PointRec::Update_I(I);
		break;
	case XYZRGB:
		in.getLine(str, '\n');
		sscanf(str.c_str(), "%lf %lf %lf %lf %lf %lf", &x, &y, &z, &r, &g, &b);
		pointSet.push_back(PointRec(x,y,z,-1, r, g, b));
		break;
	case XYZIRGB:
		in.getLine(str, '\n');
		sscanf(str.c_str(), "%lf %lf %lf %lf %lf %lf %lf", &x, &y, &z, &I, &r, &g, &b);
		pointSet.push_back(PointRec(x,y,z, I, r, g, b));
		PointRec::Update_I(I);
		break;
		}
		in.frame();
	}while(!in.isEnd());
}

//////////////////////////////////////////////////////////////////////////
//							SimpleReadStrategy
//////////////////////////////////////////////////////////////////////////
struct SimpleInputor {
	int _num;
	SimpleInputor(std::ifstream& file) : infile(file) {}
	std::ifstream& infile;
	inline void getLine(string& str, char c) { str.clear(); getline(infile, str, c); }
	inline void frame() { --_num; }
	inline bool isEnd() { return _num<=0 || !infile; }
	inline void ReadHead()
	{
		string str;
		unsigned int num = 0;
		getLine(str, '\n');
		sscanf(str.c_str(), "%u", &num);
		_num=num;
	}
};

bool SimpleReadStrategy::Init(const char* fileName, unsigned int recordType)
{
	_recordType = recordType;
	_fileName = string(fileName);
	_isFinish = false;
	return true;
}

bool SimpleReadStrategy::Read(PointSet &pointSet)
{
	if(_isFinish)
		return false;

	time_t sT, eT;

	sT = clock();

	std::ifstream infile(_fileName.c_str());

	if(!infile)
	{
		_log << "Can not open this file in this path"<<endl;
		return false;
	}

	_log<<"Begin to read data."<<endl;

	SimpleInputor inputor(infile);
	inputor.ReadHead();
	ReadConcept(inputor, pointSet, _recordType);
	infile.close();

	//inputor.infile.close();
	_log<<"All "<<pointSet.size()<<" points have been read successfully!"<<endl;

	eT = clock();
	_log << "Read-Time: "<<difftime(eT, sT)<<endl;

	_isFinish = true;
	return true;
}

void SimpleReadStrategy::Close()
{

}

//////////////////////////////////////////////////////////////////////////
//								MemMapStrategy
//////////////////////////////////////////////////////////////////////////
#include <Windows.h>
namespace FC {
	struct MemMapInputor {
	private:
		HANDLE hFile;
		DWORD size_low,size_high;
		HANDLE hMapFile;
		void* pvFile;
		char *p;
		char *pEnd;
	public:
		int _num;
		MemMapInputor() { pvFile=p=0; hFile=hMapFile=0; }
		bool init(const char *name)
		{
			//��ʼ
			//����ļ����
			hFile=CreateFile(
				name,					//�ļ���
				GENERIC_READ,			//���ļ����ж�����
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,		//���Ѵ����ļ�
				FILE_ATTRIBUTE_NORMAL,
				0);

			//����ֵsize_high,size_low�ֱ��ʾ�ļ���С�ĸ�32λ/��32λ
			size_low= GetFileSize(hFile,&size_high); 

			//�����ļ����ڴ�ӳ���ļ���  
			hMapFile=CreateFileMapping( 
				hFile,
				NULL,
				PAGE_READONLY,  //��ӳ���ļ����ж�
				size_high,
				size_low,				//������������64λ������֧�ֵ�����ļ�����Ϊ16EB
				NULL);  
			if(hMapFile==INVALID_HANDLE_VALUE)  
			{  
				cout << "Can't create file mapping.Error" << GetLastError() << endl;
				CloseHandle(hFile);
				return false;
			} 

			//���ļ�����ӳ�䵽���̵ĵ�ַ�ռ�
			pvFile=MapViewOfFile(
				hMapFile,
				FILE_MAP_READ,
				0,
				0,
				0);
			p=(char*)pvFile;
			pEnd = p + size_low;


			return true;
		}

		void close()
		{
			//����
			UnmapViewOfFile(pvFile);	//����ӳ��
			CloseHandle(hFile);				//�ر��ļ�
		}

		void getLine(string& str, char c)
		{
			str.clear();
			while(p<=pEnd && *p==c) ++p;//skip all forward c
			while(p<=pEnd && *p!=c) { str.push_back(*p++); }
		}

		inline void frame() { --_num; }
		inline bool isEnd() { return !(p<=pEnd && _num>0); }
		inline bool reachFileEnd() { return p>pEnd; }
	};
}

bool MemMapStrategy::Init(const char* fileName, unsigned int recordType)
{
	_recordType = recordType;
	_isFinish = false;
	
	inputor = new MemMapInputor;
	time_t sT, eT;
	sT = clock();
	bool ret = inputor->init(fileName);
	eT = clock();
	_log << "Map-Time: "<<difftime(eT, sT)<<endl;

	if(ret) {
		string str;
		unsigned int num = 0;
		inputor->getLine(str, '\n');
		sscanf(str.c_str(), "%u", &num);
		_numOfPointsToRead = num;
	}

	return ret;
}

//make sure everytime the pointSet is cleared at first;
bool MemMapStrategy::Read(PointSet &pointSet)
{
	if(_isFinish)
		return false;

	unsigned int numLeft;
	unsigned int thisTimeToRead;
	if(_numOfPointsToRead>MEM_MAP_THRESHOLD) {
		numLeft = _numOfPointsToRead - MEM_MAP_THRESHOLD;
		thisTimeToRead = MEM_MAP_THRESHOLD;
	} else {
		numLeft = 0;
		thisTimeToRead = _numOfPointsToRead;
	}

	time_t sT, eT;
	sT = clock();
	inputor->_num = thisTimeToRead;
	ReadConcept(*inputor, pointSet, _recordType);
	eT = clock();
	_log << "Read "<< pointSet.size() <<" points, Time : "<<difftime(eT, sT)<<endl;

	_numOfPointsToRead = numLeft;
	if(_numOfPointsToRead<=0) {
		_log << "Finish Read!"<<endl;
		_isFinish = true;
	}

	return true;
}

void MemMapStrategy::Close()
{
	if(inputor) {
		inputor->close();
		delete inputor;
		inputor = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
//                   UnknownNumStrategy
//////////////////////////////////////////////////////////////////////////
bool UnknownNumStrategy::Init(const char *fileName, unsigned int recordType)
{
	_recordType = recordType;
	_isFinish = false;

	inputor = new MemMapInputor;
	time_t sT, eT;
	sT = clock();
	bool ret = inputor->init(fileName);
	eT = clock();
	_log << "Map-Time: "<<difftime(eT, sT)<<endl;

	return ret;
}

bool UnknownNumStrategy::Read(PointSet& pointSet)
{
	if(_isFinish)
		return false;

	unsigned int numLeft;
	unsigned int thisTimeToRead = MEM_MAP_THRESHOLD;

	time_t sT, eT;
	sT = clock();
	inputor->_num = thisTimeToRead;
	ReadConcept(*inputor, pointSet, _recordType);
	eT = clock();
	_log << "Read "<< pointSet.size() <<" points, Time : "<<difftime(eT, sT)<<endl;

	if(inputor->reachFileEnd()) {
		_log << "Finish Read!"<<endl;
		_isFinish = true;
	}

	return true;
}

void UnknownNumStrategy::Close()
{
	if(inputor) {
		inputor->close();
		delete inputor;
		inputor = 0;
	}
}