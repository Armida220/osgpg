#pragma once

#include "pointRecord.h"
#include "RawPointReadStrategy.h"
#include <vector>
using namespace std;

namespace FC{
	class RawPointReader
	{
	public:
		RawPointReader(void);
		~RawPointReader(void);

		bool Init(const char* fileName, unsigned int &recordType);
		bool Read(PointSet &pointSet);
		void Close();
	protected:
		RawPointReadStrategy* _readStrategy;
	};
}

