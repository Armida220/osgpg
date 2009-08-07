#include "CamCalibrator.h"

#include <vgl/vgl_fwd.h>
#include "vnl\vnl_quaternion.h"
#include "gel\mrc\vpgl\vpgl_perspective_camera.h"
#include "gel\mrc\vpgl\algo\vpgl_optimize_camera.h"
using namespace std;

#include <vcl_iostream.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_vector_2d.h>
#include <vgl/vgl_vector_3d.h>
#include <vgl/vgl_homg_point_2d.h>
#include <vgl/vgl_homg_point_3d.h>
#include <vgl/algo/vgl_rotation_3d.h>
#include <vpgl/algo/vpgl_optimize_camera.h>
#include <vnl/vnl_double_3.h>
#include <vnl/vnl_random.h>
#include <vnl/vnl_math.h> // for pi

using namespace FC;

//////////////////////////////////////////////////////////////////////////
//from exterior element of photogrametry to rotation matrix
//extElm = { phi0, omega0, kaph0 }
vgl_rotation_3d<double> ExtElm2RotMat(double* extElm)
{
	double phi0 = extElm[0]; double omega0 = extElm[1]; double kaph0 = extElm[2];

	double a1 = cos(phi0) * cos(kaph0) - sin(phi0) * sin(omega0) * sin(kaph0);
	double a2 = - cos(phi0) * sin(kaph0) - sin(phi0) * sin(omega0) * cos(kaph0);
	double a3 = - sin(phi0) * cos(omega0);

	double b1 = cos(omega0) * sin(kaph0);
	double b2 = cos(omega0) * cos(kaph0);
	double b3 = - sin(omega0);

	double c1 = sin(phi0) * cos(kaph0) + cos(phi0) * sin(omega0) * sin(kaph0);
	double c2 = - sin(phi0) * sin(kaph0) + cos(phi0) * sin(omega0) * cos(kaph0);
	double c3 = cos(phi0) * cos(omega0);

	vnl_matrix_fixed<double,3,3> rot;
	rot[0][0] = a1; rot[0][1] = a2; rot[0][2] = a3;
	rot[1][0] = b1; rot[1][1] = b2; rot[1][2] = b3;
	rot[2][0] = c1; rot[2][1] = c2; rot[2][2] = c3;

#define OUT_ROTMAT 0
#if OUT_ROTMAT
	{
		std::ofstream outMat("rotation.log");
		outMat<<a1<<"\t"<<a2<<"\t"<<a3<<endl;
		outMat<<b1<<"\t"<<b2<<"\t"<<b3<<endl;
		outMat<<c1<<"\t"<<c2<<"\t"<<c3<<endl;
		outMat.close();
	}
#endif
	return vgl_rotation_3d<double>(rot);
}

//////////////////////////////////////////////////////////////////////////

void FC::calibrate(K_Matrix k, Uint correspondentNum, 
							 ImagePointArr imgPts, WorldPointArr wldPts,
							 R_Matrix& r_out, C_Matrix& c_out, PrincipleAxis& pa_out, P_Matrix& p_out)
{
	vcl_vector<vgl_homg_point_3d<double> > world(correspondentNum);
	vcl_vector<vgl_point_2d<double> > image(correspondentNum);
	for(Uint i=0; i<correspondentNum; ++i) {
		WorldPoint& wp = wldPts[i];
		ImagePoint& ip = imgPts[i];
		world[i] = vgl_homg_point_3d<double>( wp[0], wp[1], wp[2] );
		image[i] = vgl_point_2d<double>( ip[0], ip[1] );
	}

	vpgl_calibration_matrix<double> K(1,
		vgl_homg_point_2d<double>(k[2], k[3]), k[0], k[1]);
	vgl_homg_point_3d<double> c(10, 10, 10);
	double extElm[3] = {0 , 0, 0};
	vgl_rotation_3d<double> R = ExtElm2RotMat(extElm);
	vpgl_perspective_camera<double> InitCam(K,c,R);


	vpgl_perspective_camera<double> opt_cam = 
		vpgl_optimize_camera::opt_orient_pos(InitCam,world,image);

	vnl_matrix_fixed<double,3,3> newR = opt_cam.get_rotation().as_matrix();
	for(int i=0; i<3; ++i)
		for(int j=0; j<3; ++j)
			r_out[i][j] = newR[i][j];

	vgl_point_3d<double> newC = opt_cam.get_camera_center();
	c_out[0] = newC.x();
	c_out[1] = newC.y();
	c_out[2] = newC.z();

	vgl_vector_3d<double> newPa = opt_cam.principal_axis();
	pa_out[0] = newPa.x();
	pa_out[1] = newPa.y();
	pa_out[2] = newPa.z();

	vnl_matrix_fixed<double,3,4> newP = opt_cam.get_matrix();
	for(int i=0; i<3; ++i)
		for(int j=0; j<4; ++j)
			p_out[i][j] = newP[i][j];
}