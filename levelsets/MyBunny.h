
# include <iostream>

# include "Types.h"
# include "Vector.h"

using namespace std;

/*
VolumeFloatPtr finalDensityPtr;
VolumeColorPtr finalEmColorPtr;
VolumeColorPtr finalMatColorPtr;
BBox finalBBox;
float K;
*/

void loadBunny(VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr, VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float K)
{
	string bunnyPath = "./models/cleanbunny.obj";
	// load bunny model
	static PolyModel polyBunny;
	polyBunny.loadObj(bunnyPath);
	cout << "--------------------------------------------" << endl;
	// generate bunny levelsets
	cout << "Create levelsets..." << endl;
	static PolyLevelsets bunnyLevelsets(true, polyBunny, 3, 0.005);
	static FloatGrid::Ptr bunnyGrid = bunnyLevelsets.getLevelsets();

	// generate bunny volume
	cout << "Stamping model..." << endl;
	static VDBLevelsetsVolume bunnyVolume(bunnyGrid);
	// generate bunny BBox
	static Vec3s bunnyLLC(polyBunny.x_min, polyBunny.y_min, polyBunny.z_min);
	static Vec3s bunnyURC(polyBunny.x_max, polyBunny.y_max, polyBunny.z_max);
	static BBox bunnyBBox(bunnyLLC, bunnyURC);

	// create bunny color volume and density volume
	static Color matColor(1.0, 1.0, 1.0, 1.0);
	static Color emColor(0.0, 0.0, 0.0, 0.0);
	static ConstantColor bunnyMatColor(matColor);
	static ConstantColor bunnyEmColor(emColor);
	static ConstantFloat rho(500.0);
	static DensityVolume bunnyDensity(rho, bunnyVolume);

	// set K
	K = 0.8;
	finalDensityPtr = &bunnyDensity;
	finalEmColorPtr = &bunnyEmColor;
	finalMatColorPtr = &bunnyMatColor;
	finalBBox = bunnyBBox;

	Vector x(0, 0, 0);
	cout << finalDensityPtr->eval(x) << endl;
}

