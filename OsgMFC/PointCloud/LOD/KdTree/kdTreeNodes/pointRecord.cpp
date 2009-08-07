#include "pointRecord.h"

float PointRec::maxI;
float PointRec::minI;

void PointRec::Reset_I(float newMaxI, float newMinI)
{
	maxI = newMaxI; minI = newMinI;
}

float PointRec::GetMaxI() { return maxI; }
float PointRec::GetMinI() { return minI; }

void PointRec::Update_I(float newI)
{
	maxI = max(newI, maxI); minI = min(newI, minI);
}