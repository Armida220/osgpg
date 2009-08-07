#pragma once

#include "pointRecord.h"
#include <vector>
#include <fstream>
#include <string>
using namespace std;

namespace FC {
	class RawPointReadStrategy
	{
	public:
		virtual bool Init(const char* fileName, unsigned int recordType) = 0;
		virtual bool Read(PointSet& pointSet) = 0;
		virtual void Close() = 0;
	protected:
		std::ofstream _log;
		unsigned int	_recordType;
		bool					_isFinish;
	};

	class SimpleReadStrategy : public RawPointReadStrategy
	{
	public:
		SimpleReadStrategy() { _log.open("SimpleReadStrategy.log"); }
		~SimpleReadStrategy() { _log.close(); }

		bool Init(const char* fileName, unsigned int recordType);
		bool Read(PointSet& pointSet);
		void Close();
	protected:
		string					_fileName;
	};

	//forward declare
	struct MemMapInputor;

	class MemMapStrategy : public RawPointReadStrategy
	{
	public:
		MemMapStrategy() { _log.open("MemMapStrategy.log"); }
		~MemMapStrategy() { _log.close(); }

		bool Init(const char* fileName, unsigned int recordType);
		bool Read(PointSet& pointSet);
		void Close();
	protected:
		MemMapInputor*	inputor;
		unsigned int		_numOfPointsToRead;
	};
}
