
# include <iostream>
# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <vector>

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

# include "MyBunny.h"	// load bunny
# include "MyTeapot.h"	// load teapot
# include "MyHybrid.h"	// load hybrid

using namespace std;
using namespace lux;

# define WEIGHT 1920
# define HEIGHT 1080
# define STEP_SIZE 0.003
# define LIGHT_STEP_SIZE 0.009
# define NEAR 0.1
# define FAR 30
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
	// cmd line parser
	// usage: ./levelsets frameStart frameNum
	int frameStart = 0;
	int frameNum = 120;
	string model_name;
	float camera_x, camera_y;
	string model_tag;
	if (argc == 4)
	{	
		frameStart = atof(argv[1]);
		frameNum = atof(argv[2]);
		model_tag = argv[3];
		if (model_tag == "-b")	{model_name = "bunny"; camera_x = 3.5; camera_y = 0.0;}
		else if (model_tag == "-t")	{model_name = "teapot"; camera_x = 3.5; camera_y = 0.0;}
		else if (model_tag == "-h")	{model_name = "hybrid"; camera_x = 4.5; camera_y = 1.0;}
		else	{printHelp(); exit(0);}
	}
	else
	{
		printHelp();
		exit(0);
	}

	/// -------------------------- load model --------------------------------

	if (model_name == "bunny")
		{loadBunny(finalDensityPtr, finalEmColorPtr, finalMatColorPtr, finalBBox, K);}
	else if (model_name == "teapot")
		{loadTeapot(finalDensityPtr, finalEmColorPtr, finalMatColorPtr, finalBBox, K);}
	else if (model_name == "hybrid")
		{loadHybrid(finalDensityPtr, finalEmColorPtr, finalMatColorPtr, finalBBox, K);}

	/// ----------------------------- do lights --------------------------------

	// lighting
	cout << "Set lights..." << endl;
	std::vector<LightSource> myLights;
	// light position
	Vector keyPos(0.0, 4.0, 0.0);
	Vector rimPos(0.0, -4.0, 0.0);
	Vector backPos(0.0, 0.0, -4.0);
	// light color
	Color keyColor(0.1, 1.2, 0.53, 1.0);
	Color rimColor(0.34, 0.74, 0.86, 1.0);
	Color backColor(0.016, 0.25, 0.9, 1.0);
	// set lights
	LightSource keyLight(keyPos, keyColor);
	LightSource rimLight(rimPos, rimColor);
	LightSource backLight(backPos, backColor);
	myLights.push_back(keyLight);
	myLights.push_back(rimLight);
	myLights.push_back(backLight);
	LightVolume lightVolume(myLights, *finalDensityPtr, *finalMatColorPtr, K, LIGHT_STEP_SIZE, 0.007, finalBBox);

	/// ----------------------------- do rendering ------------------------------

	// set rendering 
	// set camera and image 
	cout << "Set image..." << endl;
	Image myImg;
	myImg.reset(WEIGHT, HEIGHT);
	cout << "Set camera..." << endl;
	Camera myCamera;
	Vector originEye(0.0, camera_y, camera_x);
	Vector lookAt(0.0, camera_y, 0.0);
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
		// write into file
		char file_name[50];
		sprintf(file_name, "./results/jingcoz_%s_hw2.%04d.exr", model_name.c_str(), frame_id);
		cout << "Write frame " << frame_id << " into" << file_name << "."<< endl;
		writeOIIOImage(file_name, myImg);
	}

	return 0;
}

