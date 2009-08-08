#include "CamParameters.h"

using namespace FC;

CamParameters::CamParameters(std::ifstream& camFile)
{
	camFile >> f;
	camFile >> dx >> dy;
	camFile >> u0 >> v0;
	camFile >> pw >> ph;
	camFile >> correspondentNum;
	for(Uint i=0; i<correspondentNum; ++i) {
		WorldPoint& wp = world[i];
		ImagePoint& ip = image[i];
		camFile>>wp[0]>>wp[1]>>wp[2]>>ip[0]>>ip[1];
	}
	camFile>>photoPath;

	k[0] = f/dx;
	k[1] = f/dy;
	k[2] = u0;
	k[3] = v0;

	needReComputeParameters = true;
}

CamParameters::CamParameters() {}

CamParameters::CamParameters(const CamParameters& rhs)
{
	*this = rhs;
}

CamParameters& CamParameters::operator=(const CamParameters& rhs)
{
	if(this == &rhs) return *this;
	f = rhs.f;
	dx = rhs.dx; dy = rhs.dy;
	u0 = rhs.u0; v0 = rhs.v0;
	pw = rhs.pw; ph = rhs.ph;
	correspondentNum = rhs.correspondentNum;
	for(Uint i=0; i<correspondentNum; ++i) {
		world[i][0] = rhs.world[i][0];
		world[i][1] = rhs.world[i][1];
		world[i][2] = rhs.world[i][2];
		image[i][0] = rhs.image[i][0];
		image[i][1] = rhs.image[i][1];
	}
	photoPath = rhs.photoPath;

	k[0] = f/dx;
	k[1] = f/dy;
	k[2] = u0;
	k[3] = v0;

	needReComputeParameters = rhs.needReComputeParameters;

	return *this;
}