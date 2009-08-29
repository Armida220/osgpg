/*
Using VXL
--Simbaforrest
*/
#ifdef CALIBRATE_EXPORTS
#define CALIBRATE_API __declspec(dllexport)
#else
#define CALIBRATE_API __declspec(dllimport)
#endif

#include <iostream>
using namespace std;

namespace FC {
	//K matrix in Computer Vision, camera interior paras
	//alphaX, alphaY, u0, v0
	typedef double K_Matrix[4];

	//R matrix, rotation of camera
	typedef double R_Matrix[3][3];

	//C matrix, center of camera
	//Xc, Yc, Zc
	typedef double C_Matrix[3];

	typedef double PrincipleAxis[3];

	typedef double P_Matrix[3][4];

	typedef double ImagePoint[2];
	typedef ImagePoint* ImagePointArr;

	typedef double WorldPoint[3];
	typedef WorldPoint* WorldPointArr;

	typedef unsigned int Uint;

	//x0, y0, z0, phi0, omega0, kappa0
	typedef double InitParameters[6];

	//init parameter is in photogrammetry system
	//calibrate system is in computer vision system
	CALIBRATE_API void calibrate(InitParameters init, 
		K_Matrix k, Uint correspondentNum,
		ImagePointArr imgPts, WorldPointArr wldPts,
		R_Matrix& r_out, C_Matrix& c_out, PrincipleAxis& pa_out, P_Matrix& p_out);
}