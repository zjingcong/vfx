
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
# include "MyHumanoid.h"	// load humanoid

using namespace std;
using namespace lux;

# define WEIGHT 320
# define HEIGHT 180
# define STEP_SIZE 0.01
# define NEAR 12
# define FAR 18

int main(int argc, char* argv[])
{
	/*
	/// --------------------------- bunny ------------------------------------

	string bunnyPath = "./models/cleanbunny.obj";
	// load bunny model
	PolyModel polyBunny;
	polyBunny.loadObj(bunnyPath);
	// generate bunny levelsets
	PolyLevelsets bunnyLevelsets(polyBunny, 4, 0.1);
	FloatGrid::Ptr bunnyGrid = bunnyLevelsets.getLevelsets();
	// generate bunny volume
	FloatGridVolume bunnyVolume(bunnyGrid);
	// generate bunny BBox
	Vec3s bunnyLLC(polyBunny.x_min, polyBunny.y_min, polyBunny.z_min);
	Vec3s bunnyURC(polyBunny.x_max, polyBunny.y_max, polyBunny.z_max);
	BBox bunnyBBox(bunnyLLC, bunnyURC);

	// create bunny color volume and density volume
	Color redColor(1.0, 0.0, 0.0, 1.0);
	ConstantColor red(redColor);
	ConstantFloat rho(10.0);
	ColorVolume finalColor(red, bunnyVolume);
	DensityVolume finalDensity(rho, bunnyVolume);

	/// ---------------------------------------------------------------------
	*/

	Vec3s humanLLC(-3.0, -6.0, -3.0);
	Vec3s humanURC(3.0, 3.0, 3.0);
	BBox humanBBox(humanLLC, humanURC);
	FloatVolumeToGrid humanVolume2Grid(humanFinalDensity, 0.1, humanBBox);
	FloatGrid::Ptr humanGrid = humanVolume2Grid.getVolumeGrid();
	FloatGridVolume finalDensity(humanGrid);
	ColorVolumeToGrid humanColor2Grid(humanFinalColor, 0.1, humanBBox);
	Vec4fGrid::Ptr humanColorGrid = humanColor2Grid.getVolumeGrid();
	ColorGridVolume finalColor(humanColorGrid);

	/// ---------------------------------------------------------------------

	// set rendering
	int frame_id = 0;
	cout << "set image..." << endl;
	Image myImg;
	myImg.reset(WEIGHT, HEIGHT);
	cout << "set camera..." << endl;
	Camera myCamera;
	Vector eye(15.0, -2.0, 0.0);
	Vector view(-1.0, 0.0, 0.0);
	Vector up(0.0, 1.0, 0.0);
	myCamera.setEyeViewUp(eye, view, up);
	myCamera.setFarPlane(NEAR);
	myCamera.setFarPlane(FAR);
	cout << "start rendering..." << endl;
	// rendering (multithreading)
	Renderer myRenderer(myImg, myCamera, STEP_SIZE);
	myRenderer.render(humanFinalColor, humanFinalDensity, humanBBox);
	cout << "rendering complete." << endl;
	// write into file
	char file_name[50];
	sprintf(file_name, "./results/jingcoz_hw2.%04d.exr", frame_id);
	cout << "Frame " << frame_id << " into" << file_name << " complete."<< endl;
	writeOIIOImage(file_name, myImg);

	return 0;
}

