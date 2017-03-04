# ifndef __MYHYBRID_H__
# define __MYHYBRID_H__

# include <iostream>
# include <openvdb/tools/Interpolation.h>
# include <openvdb/tools/GridTransformer.h>
# include <openvdb/math/Math.h>
# include <openvdb/tools/Composite.h>

# include "Types.h"
# include "Vector.h"
# include "Shape.h"
# include "Operations.h"
# include "ScalarVolumeXform.h"
# include "MyHumanoid.h"	// load humanoid
# include "MyBunny.h"	// load bunny
# include "MyTeapot.h"	// load teapot

# define PI 3.14159265

using namespace std;


void loadHybrid(VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr, VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float& K)
{
	static Color matColor(1.0, 1.0, 1.0, 1.0);
	static Color emColor(0.0, 0.0, 0.0, 0.0);
	static ConstantFloat rho_bunny(500.0);
	static ConstantFloat rho_teapot(600.0);
	static ConstantColor polyMatColor(matColor);
	static ConstantColor polyEmColor(emColor);

	/// ------------------------------- humanoid -------------------------------------

	static Vec3s humanLLC(-3.0, -6.0, -3.0);
	static Vec3s humanURC(3.0, 3.0, 3.0);
	static BBox humanBBox(humanLLC, humanURC);
	// stamp humanoid to the grid
	cout << "Stamping humaniod density..." << endl;
	static FloatVolumeToGrid humanVolume2Grid(humanFinalDensity, 0.02, humanBBox);
	FloatGrid::Ptr humanDensityGrid = humanVolume2Grid.getVolumeGrid();
	cout << "Stamping humanoid color..." << endl;
	static ColorVolumeToGrid humanColor2Grid(humanFinalColor, 0.02, humanBBox);
	Vec4fGrid::Ptr humanColorGrid = humanColor2Grid.getVolumeGrid();
	// get the transform
	Transform::Ptr humanDensityXform = humanDensityGrid -> transformPtr();
	Transform::Ptr humanColorXform = humanColorGrid -> transformPtr();
	// scale
	humanDensityXform -> postScale(0.09);
	humanColorXform -> postScale(0.09);
	humanDensityGrid -> setTransform(humanDensityXform);
	humanColorGrid -> setTransform(humanColorXform);
	//translate
	Vec3s humanTranslateVec(1.25, 0.9, 0.0);
	humanDensityXform -> postTranslate(humanTranslateVec);
	humanColorXform -> postTranslate(humanTranslateVec);
	humanDensityGrid -> setTransform(humanDensityXform);
	humanColorGrid -> setTransform(humanColorXform);
	// grid to volume
	static FloatGridVolume hybridDensity(humanDensityGrid);
	static ColorGridVolume hybridMatColor(humanColorGrid);

	/// ------------------------------- bunny & teapot---------------------------------

	static BBox bunnyBBox;
	static FloatGrid::Ptr bunnyGrid;
	bunnyGrid = loadBunnyGrid(bunnyBBox);
	// transform bunny
	Transform::Ptr bunnyXform = bunnyGrid -> transformPtr();
	// translate
	Vec3s bunnyTranslateVec(0.0, 0.7, 0.0);
	bunnyXform -> postTranslate(bunnyTranslateVec);
	bunnyGrid -> setTransform(bunnyXform);
	bunnyBBox.translate(bunnyTranslateVec);
	// scale
	bunnyXform -> postScale(0.65);
	bunnyGrid -> setTransform(bunnyXform);
	// rotation
	double radians = 3 * PI / float(4);
	bunnyXform -> postRotate(radians, openvdb::math::Y_AXIS);
	bunnyGrid -> setTransform(bunnyXform);
	cout << "-------------------------------------" << endl;
	static BBox teapotBBox;
	static FloatGrid::Ptr teapotGrid;
	teapotGrid = loadTeapotGrid(teapotBBox);
	cout << "-------------------------------------" << endl;
	// generate model volume
	cout << "Stamping model bunny..." << endl;
	static VDBLevelsetsVolume bunnyVolume(bunnyGrid);
	static DensityVolume bunnyDensity(rho_bunny, bunnyVolume);
	cout << "Stamping model teapot..." << endl;
	static VDBLevelsetsVolume teapotVolume(teapotGrid);
	static DensityVolume teapotDensity(rho_teapot, teapotVolume);
	// remove the lid of the teapot
	static Box box(0.8, 6);
	static ScalarTranslate myCuttingBox(box, Vector(0.0, 1.5, 0.0));
	static ImplicitCutout newTeapot(teapotDensity, myCuttingBox);

	/// ----------------------------- composition -------------------------------------

	// union the density
	static ImplicitUnion bunnyAndTeapot(newTeapot, bunnyDensity);
	static ImplicitUnion allDensity(bunnyAndTeapot, hybridDensity);
	// union the color
	static ColorVolume bunnyTeapotMatColor(polyMatColor, bunnyAndTeapot);
	static ColorAdd allMatColor(bunnyTeapotMatColor, hybridMatColor);
	// union the bbox
	static BBox myfinalBbox;
	teapotBBox.expand(bunnyBBox);
	myfinalBbox = teapotBBox;

	// std::cout << myfinalBbox.min() << std::endl;
	// std::cout << myfinalBbox.max() << std::endl;

	cout << "Stamping final model density..." << endl;
	static FloatVolumeToGrid finalVolume2Grid(allDensity, 0.004, myfinalBbox);
	FloatGrid::Ptr finalDensityGrid = finalVolume2Grid.getVolumeGrid();
	static FloatGridVolume finalDensity(finalDensityGrid);
	cout << "Stamping final model color..." << endl;
	static ColorVolumeToGrid finalColor2Grid(allMatColor, 0.004, myfinalBbox);
	Vec4fGrid::Ptr finalColorGrid = finalColor2Grid.getVolumeGrid();
	static ColorGridVolume finalMatColor(finalColorGrid);

	K = 1;
	finalDensityPtr = &finalDensity;
	finalEmColorPtr = &polyEmColor;
	finalMatColorPtr = &finalMatColor;
	finalBBox = myfinalBbox;
}

# endif

