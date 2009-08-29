
#pragma once
#include <math.h>
#include <cxcore.h>
#include <cv.h>
#include "Geometry.h"
#include <vector>
using namespace std;

namespace Ransac {
	typedef vector<int> ObserveIdxArr;

	struct ObserveDataSet {
		vector<Point2D> pts;
		void RandGenMinDataSet(ObserveIdxArr& mds)
		{
			mds.clear();
			unsigned int size = pts.size();
			mds.reserve(3);
			for(int i=0; i<3; ++i) {
				int idx = rand()%size;
				mds.push_back(idx);
			}
		}
	};
	
	struct CircleModel {
		static unsigned int n;
		DataType r;
		Point2D  center;
		void ComputeConsensusSet(const ObserveDataSet& ods, 
			ObserveIdxArr& consensusSet, DataType thresh)
		{
			consensusSet.clear();
			for(unsigned int i=0; i<ods.pts.size(); ++i)
			{
				const Point2D& pt = ods.pts[i];
				if( abs(pt.DistanceTo(center) - r) < thresh ) {
					consensusSet.push_back(i);
				}
			}
		}
	};
	unsigned int CircleModel::n = 3;

	void EstimateFromConsensusSet(const ObserveDataSet& ods, 
			const ObserveIdxArr& consensusSet, 
			CircleModel& m)
	{
			unsigned int n = consensusSet.size();
			DataType* a = new DataType[n*n];
			DataType* b = new DataType[n];
			memset(a, 0, n*n*sizeof(DataType));
			memset(b, 0, n*sizeof(DataType));

			for(unsigned int i=0; i<n; ++i)
			{
				const Point2D& pt = ods.pts[consensusSet[i]];
				a[i*n  ] = pt.x;
				a[i*n+1] = pt.y;
				a[i*n+2] = 1;

				b[i] = -pt.x*pt.x-pt.y*pt.y;
			}

			CvMat *AA=cvCreateMat(n,n,CV_64FC1);
			CvMat *BB=cvCreateMat(n,1,CV_64FC1);
			CvMat *X=cvCreateMat(n,1,CV_64FC1);

			cvSetData(AA,a,AA->step);
			cvSetData(BB,b,BB->step);
			cvSolve(AA,BB,X,CV_SVD);

			DataType D = X->data.db[0];
			DataType E = X->data.db[1];
			DataType F = X->data.db[2];

			delete [] a;
			delete [] b;

			m.center.x = -D/2;
			m.center.y = -E/2;
			m.r = sqrt(m.center.x*m.center.x+m.center.y*m.center.y-F);
	}

	
	struct CircleRansacor
	{
		inline double calcK(double z, double w, int n)
		{
			return log(z)/log(1-pow(w, n));
		}
		
		CircleModel Estimate(ObserveDataSet& ods, 
			DataType thresh_t, DataType thresh_z)
		{
			CircleModel ret;
			ObserveIdxArr maxConsensusSet;
			ObserveIdxArr minSet;
			ObserveIdxArr consensusSet;
			unsigned int cnt=0;
			double K,w=0.1;
			K = calcK(thresh_z, w, CircleModel::n);

			while(1) {
				if(cnt>=K) break;

				ods.RandGenMinDataSet(minSet);

				EstimateFromConsensusSet(ods, minSet, ret);

				ret.ComputeConsensusSet(ods, consensusSet, thresh_t);

				double wt = double(consensusSet.size())/ods.pts.size();
				if(w<wt) {
					w=wt;
					K=calcK(thresh_z, w, CircleModel::n);
				}

				if(consensusSet.size()>maxConsensusSet.size()) {
					maxConsensusSet = consensusSet;
				}
				cnt++;
			}
			EstimateFromConsensusSet(ods, maxConsensusSet, ret);

			return ret;
		}
	};
}