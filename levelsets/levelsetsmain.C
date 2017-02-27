
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
# define STEP_SIZE 0.01
# define NEAR 2
# define FAR 10

int main(int argc, char* argv[])
{
	string bunnyPath = "./models/bunny.obj";
	// load bunny model
	PolyModel polyBunny;
	polyBunny.loadObj(bunnyPath);
	// generate bunny levelsets
	PolyLevelsets bunnyLevelsets(polyBunny, 3, 0.01);
	FloatGrid::Ptr bunnyGrid = bunnyLevelsets.getLevelsets();
	// generate bunny volume
	FloatGridVolume bunnyVolume(bunnyGrid);

	/// ---------------------------------------------------------------------

	// create bunny color volume and density volume
	Color redColor(1.0, 0.0, 0.0, 1.0);
	ConstantColor red(redColor);
	ConstantFloat rho(100);
	ColorVolume finalColor(red, bunnyVolume);
	DensityVolume finalDensity(rho, bunnyVolume);
	// set rendering
	int frame_id = 0;
	cout << "set image..." << endl;
	Image myImg;
	myImg.reset(WEIGHT, HEIGHT);
	cout << "set camera..." << endl;
	Camera myCamera;
	Vector eye(0.0, 0.5, 3.0);
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

