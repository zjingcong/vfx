
# include <iostream>
# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <vector>

# include <openvdb/tools/Interpolation.h>	// delete later
# include <openvdb/tools/GridTransformer.h>	// delete later

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
# include "Lighting.h"
# include "Structs.h"
# include "Volume.h"
# include "MyHumanoid.h"	// load humanoid
# include "MyBunny.h"	// load bunny

using namespace std;
using namespace lux;

# define WEIGHT 320
# define HEIGHT 180
# define STEP_SIZE 0.01
# define NEAR 0.1
# define FAR 10
# define PI 3.14159265


VolumeFloatPtr finalDensityPtr;
VolumeColorPtr finalEmColorPtr;
VolumeColorPtr finalMatColorPtr;
BBox finalBBox;
float K;


void printHelp()
{
	cout << "[Usage] ./levelsets frameStart frameNum [model_tag]" << endl;
	cout << "====================================================" << endl;
	cout << "[model_tag]: " << endl;
	cout << "\t -b: bunny\n\t -t: teapot\n\t -h: hybrid" << endl;
}


// theta in degree
// axis should be unit vector
Vector vecRotation(Vector v, Vector axis, float a)
{
	float theta = PI * a / 180.0;
	return v * cos(theta) + axis * (axis * v) * ( 1 - cos(theta)) + (axis ^ v) * sin(theta);
}


int main(int argc, char* argv[])
{
	// usage: ./levelsets frameStart frameNum
	int frameStart = 0;
	int frameNum = 120;
	string model_name;
	string model_tag;
	if (argc == 4)
	{	
		frameStart = atof(argv[1]);
		frameNum = atof(argv[2]);
		model_tag = argv[3];
		if (model_tag == "-b")	{model_name = "bunny";}
		else if (model_tag == "-t")	{model_name = "teapot";}
		else if (model_tag == "-h")	{model_name = "hybrid";}
		else	{printHelp(); exit(0);}
	}
	else
	{
		printHelp();
		exit(0);
	}


/// ---------------- grid transform area ----------------------------------
/*
	Transform::Ptr bunnyXform = bunnyGrid -> transformPtr();
	// bunnyXform -> postTranslate(Vec3s(0.0, 1.0, 0.0));
	bunnyXform -> postScale(0.5);
	bunnyGrid -> setTransform(bunnyXform);
*/
/// ----------------- grid transform area ---------------------------------

	/// ---------------------------------------------------------------------

/*
	Vec3s humanLLC(-3.0, -6.0, -3.0);
	Vec3s humanURC(3.0, 3.0, 3.0);
	BBox humanBBox(humanLLC, humanURC);
	FloatVolumeToGrid humanVolume2Grid(humanFinalDensity, 0.1, humanBBox);
	FloatGrid::Ptr humanGrid = humanVolume2Grid.getVolumeGrid();
	FloatGridVolume finalDensity(humanGrid);
	ColorVolumeToGrid humanColor2Grid(humanFinalColor, 0.1, humanBBox);
	Vec4fGrid::Ptr humanColorGrid = humanColor2Grid.getVolumeGrid();
	ColorGridVolume finalColor(humanColorGrid);
*/

	/// ---------------------------------------------------------------------


	string bunnyPath = "./models/cleanbunny.obj";
	// load bunny model
	PolyModel polyBunny;
	polyBunny.loadObj(bunnyPath);
	cout << "--------------------------------------------" << endl;

	// generate bunny levelsets
	cout << "Create levelsets..." << endl;
	PolyLevelsets bunnyLevelsets(true, polyBunny, 3, 0.005);
	FloatGrid::Ptr bunnyGrid = bunnyLevelsets.getLevelsets();

	// generate bunny volume
	cout << "Stamping model..." << endl;
	VDBLevelsetsVolume bunnyVolume(bunnyGrid);

	// generate bunny BBox
	Vec3s bunnyLLC(polyBunny.x_min, polyBunny.y_min, polyBunny.z_min);
	Vec3s bunnyURC(polyBunny.x_max, polyBunny.y_max, polyBunny.z_max);
	BBox bunnyBBox(bunnyLLC, bunnyURC);


	// create bunny color volume and density volume
	Color matColor(1.0, 1.0, 1.0, 1.0);
	Color emColor(0.0, 0.0, 0.0, 0.0);
	ConstantColor bunnyMatColor(matColor);
	ConstantColor bunnyEmColor(emColor);
	ConstantFloat rho(500.0);
	DensityVolume bunnyDensity(rho, bunnyVolume);

	finalDensityPtr = &bunnyDensity;
	finalEmColorPtr = &bunnyEmColor;
	finalMatColorPtr = &bunnyMatColor;
	finalBBox = bunnyBBox;

	float K = 0.8;
	Vector x(0, 0, 0);
	cout << finalDensityPtr->eval(x) << endl;


	// loadBunny(finalDensityPtr, finalEmColorPtr, finalMatColorPtr, finalBBox, K);
	// Vector x(0, 0, 0);
	// cout << finalDensityPtr->eval(x) << endl;

	/// ---------------------------------------------------------------------------------

	// lighting
	cout << "Set lights..." << endl;
	std::vector<LightSource> myLights;
	// light position
	Vector keyPos(0.0, 15.0, 0.0);
	Vector rimPos(0.0, -15.0, 0.0);
	Vector backPos(0.0, 0.0, -15.0);
	// light color
	Color keyColor(1.5, 1.5, 1.5, 1.0);
	Color rimColor(0.1, 0.1, 0.1, 1.0);
	Color backColor(1.0, 0.0, 0.0, 1.0);
	// set lights
	LightSource keyLight(keyPos, keyColor);
	LightSource rimLight(rimPos, rimColor);
	LightSource backLight(backPos, backColor);
	myLights.push_back(keyLight);
	myLights.push_back(rimLight);
	myLights.push_back(backLight);
	LightVolume lightVolume(myLights, *finalDensityPtr, *finalMatColorPtr, K, STEP_SIZE, 0.1, finalBBox);
	// LightVolume lightVolume(myLights, bunnyDensity, bunnyMatColor, K, STEP_SIZE, 0.1, bunnyBBox);
	// set rendering
	// set camera and image
	cout << "Set image..." << endl;
	Image myImg;
	myImg.reset(WEIGHT, HEIGHT);
	cout << "Set camera..." << endl;
	Camera myCamera;
	Vector originEye(0, 0, 4.5);
	Vector lookAt(0.0, 0.0, 0.0);
	Vector up(0.0, 1.0, 0.0);
	myCamera.setFarPlane(NEAR);
	myCamera.setFarPlane(FAR);
	cout << "=====================================" << endl;

	for (int i = 0; i < frameNum; ++i)
	{
		// get camera rotation
		int frame_id = i + frameStart;
		float angle = float(360) / 120;	// total frame: 120
		float theta = frame_id  * angle;
		Vector lookAt(0.0, 0.0, 0.0);
		Vector camera_axis(0.0, 1.0, 0.0);

		// get camera position and camera view
		Vector eye_new = vecRotation(originEye, camera_axis, theta);
		Vector view_new = lookAt - eye_new;
		myCamera.setEyeViewUp(eye_new, view_new, up);

		// rendering (multithreading)
		cout << "Start rendering frame " << frame_id << "..." << endl;
		Renderer myRenderer(myImg, myCamera, STEP_SIZE);
		myRenderer.render(*finalEmColorPtr, *finalDensityPtr, K, lightVolume, finalBBox, 1);
		// myRenderer.render(bunnyEmColor, bunnyDensity, K, lightVolume, bunnyBBox, 1);
		// write into file
		char file_name[50];
		sprintf(file_name, "./results/jingcoz_%s_hw2.%04d.exr", model_name.c_str(), frame_id);
		cout << "Write frame " << frame_id << " into" << file_name << "."<< endl;
		writeOIIOImage(file_name, myImg);
	}

	return 0;
}

