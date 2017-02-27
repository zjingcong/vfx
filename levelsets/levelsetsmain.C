
# include <iostream>
# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <vector>
# include <openvdb/tools/LevelSetSphere.h>	// delete later
# include <openvdb/tools/Interpolation.h>	// delete later

# include "Types.h"	// openvdb type define
# include "Vector.h"
# include "PolyModel.h"
# include "Grid.h"
# include "Levelsets.h"
# include "Renderer.h"
# include "OIIOFiles.h"
# include "Camera.h"
# include "PropertyVolume.h"
# include "Color.h"

using namespace std;
using namespace lux;

# define WEIGHT 320
# define HEIGHT 180
# define STEP_SIZE 0.1
# define NEAR 0.1
# define FAR 10

int main(int argc, char* argv[])
{
	string bunnyPath = "./models/bunny.obj";
	// load bunny model
	PolyModel polyBunny;
	polyBunny.loadObj(bunnyPath);
	// generate bunny levelsets
	PolyLevelsets bunnyLevelsets(polyBunny, 3, 0.004);
	FloatGrid::Ptr bunnyGrid = bunnyLevelsets.getLevelsets();
	// generate bunny volume
	FloatGridVolume bunnyVolume(bunnyGrid);

/*
	FloatGrid::Ptr grid = FloatGrid::create(0);
	grid -> setBackground(0);
	Transform::Ptr transform = grid->transformPtr();
	// create a linear transform that sets the voxel size of the grid to 0.1
  transform = Transform::createLinearTransform(0.1);
	FloatGrid::Accessor accessor = grid->getAccessor();
	Coord ijk0(0, 0, 0);
	Coord ijk1(0, 0, 1);
	accessor.setValue(ijk0, -1);
	accessor.setValue(ijk1, -1);
	Vec3s pos(0, 0, 0.3);
	Coord newijk = transform->worldToIndexNodeCentered(pos);
	cout << "coord: " << newijk << endl;
	cout << "pos(0, 0, 0.3): " << accessor.getValue(newijk) << endl;

	openvdb::tools::GridSampler<FloatGrid, openvdb::tools::BoxSampler> sampler(*grid);
	std::cout << "sampler: " << sampler.wsSample(pos) << std::endl;
*/

/*
	FloatGrid::Ptr grid = openvdb::tools::createLevelSetSphere<FloatGrid>(50.0, Vec3f(0,0,0), 0.5);
	Transform::Ptr transform = grid->transformPtr();
	FloatGrid::Accessor accessor = grid->getAccessor();
	Vec3s pos(0, 49, 0);
	Coord newijk = transform->worldToIndexNodeCentered(pos);
	cout << "coord: " << newijk << endl;
	cout << "pos(0, 49, 0): " << accessor.getValue(newijk) << endl;
	openvdb::tools::GridSampler<FloatGrid, openvdb::tools::PointSampler> sampler(*grid);
	std::cout << "sampler: " << sampler.wsSample(pos) << std::endl;
*/


/*
	Vector x(0, 1, 0);
	cout << bunnyVolume.eval(x) << endl;
	Vector y(0, 0, 0);
	cout << bunnyVolume.eval(y) << endl;
	Vector z(0, 2, 0);
	cout << bunnyVolume.eval(z) << endl;
*/

	Color redColor(100, 100, 100, 1.0);
	ConstantColor red(redColor);
	ConstantFloat rho(100);
	ColorVolume finalColor(red, bunnyVolume);
	DensityVolume finalDensity(rho, bunnyVolume);

	int frame_id = 0;

	cout << "set image..." << endl;
	Image myImg;
	myImg.reset(WEIGHT, HEIGHT);
	cout << "set camera..." << endl;
	Camera myCamera;
	Vector eye(0.0, 0.5, 4.0);
	Vector view(0.0, 0.0, -1.0);
	Vector up(0.0, 1.0, 0.0);
	myCamera.setEyeViewUp(eye, view, up);
	myCamera.setFarPlane(NEAR);
	myCamera.setFarPlane(FAR);
	cout << "start rendering..." << endl;
	// rendering (multithreading)
	Renderer myRenderer(myImg, myCamera, STEP_SIZE);
	myRenderer.render(finalColor, finalDensity);
	cout << "rendering complete." << endl;
	// write into file
	char file_name[50];
	sprintf(file_name, "./results/jingcoz_hw2.%04d.exr", frame_id);
	cout << "Frame " << frame_id << " into" << file_name << " complete."<< endl;
	writeOIIOImage(file_name, myImg);

	return 0;
}

