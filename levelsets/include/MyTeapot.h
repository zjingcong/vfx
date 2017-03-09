# ifndef __MYTEAPOT_H__
# define __MYTEAPOT_H__

# include <iostream>

# include "Types.h"
# include "Vector.h"

using namespace std;


void loadTeapot(VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr, VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float& K)
{
	string teapotPath = "./models/cleanteapot.obj";
	// load teapot model
	static PolyModel polyTeapot;
	polyTeapot.loadObj(teapotPath);
	cout << "-------------------------------------" << endl;
	// generate teapot levelsets
	cout << "Create levelsets..." << endl;
	static PolyLevelsets teapotLevelsets(true, polyTeapot, 3, 0.005);
	static FloatGrid::Ptr teapotGrid = teapotLevelsets.getLevelsets();

	// generate teapot volume
	cout << "Stamping model..." << endl;
	static VDBLevelsetsVolume teapotVolume(teapotGrid);
	// generate teapot BBox
	static Vec3s teapotLLC(polyTeapot.x_min, polyTeapot.y_min, polyTeapot.z_min);
	static Vec3s teapotURC(polyTeapot.x_max, polyTeapot.y_max, polyTeapot.z_max);
	static BBox teapotBBox(teapotLLC, teapotURC);

	// create teapot color volume and density volume
	static Color matColor(1.0, 1.0, 1.0, 1.0);
	static Color emColor(0.0, 0.0, 0.0, 0.0);
	static ConstantColor teapotMatColor(matColor);
	static ConstantColor teapotEmColor(emColor);
	static ConstantFloat rho(700.0);
	static DensityVolume teapotDensity(rho, teapotVolume);

	// set K
	K = 0.8;
	finalDensityPtr = &teapotDensity;
	finalEmColorPtr = &teapotEmColor;
	finalMatColorPtr = &teapotMatColor;
	finalBBox = teapotBBox;
}


FloatGrid::Ptr loadTeapotGrid(BBox& bbox)
{
	string teapotPath = "./models/cleanteapot.obj";
	// load teapot model
	static PolyModel polyTeapot;
	polyTeapot.loadObj(teapotPath);
	cout << "-------------------------------------" << endl;
	// generate teapot levelsets
	cout << "Create levelsets..." << endl;
	static PolyLevelsets teapotLevelsets(true, polyTeapot, 3, 0.005);
	static FloatGrid::Ptr teapotGrid = teapotLevelsets.getLevelsets();

	// generate teapot BBox
	static Vec3s teapotLLC(polyTeapot.x_min, polyTeapot.y_min, polyTeapot.z_min);
	static Vec3s teapotURC(polyTeapot.x_max, polyTeapot.y_max, polyTeapot.z_max);
	static BBox teapotBBox(teapotLLC, teapotURC);

	bbox = teapotBBox;

	return teapotGrid;
}

# endif

