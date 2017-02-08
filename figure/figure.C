# include "Shape.h"
# include "Vector.h"
# include "Camera.h"
# include "Renderer.h"
# include "OIIOFiles.h"
# include "PropertyVolume.h"

# include <iostream>
# include <stdlib.h>

using namespace lux;
using namespace std;

# define STEP_SIZE 0.3
# define WEIGHT 100
# define HEIGHT 100


int main()
{
	// volume setup
	Vector sphere_pos(0.0, 0.0, 0.0);
	Sphere sphere(sphere_pos, 10.0);
	Color redColor(255.0, 0.0, 0.0, 255.0);
	ColorVolume sphereColor(sphere, redColor);
	float density = 1.0;
	DensityVolume sphereDensity(sphere, density);
	// camera setup
	Camera myCamera;
	Vector eye(30.0, 0.0, 0.0);
  Vector view(-1.0, 0.0, 0.0);
  Vector up(0.0, 1.0, 0.0);
  myCamera.setEyeViewUp(eye, view, up);
  // renderer setup
	Image myImg;
	myImg.reset(WEIGHT, HEIGHT);
  Renderer myRenderer(myImg, myCamera, STEP_SIZE);
	// rendering
	myRenderer.render(sphere, sphereColor, sphereDensity);
	// write into file
	cout << "Write exr image into " << "test.exr" << endl;
	writeOIIOImage("test.exr", myImg);

	/*
	Vector x(5.0, 0.0, 0.0);
	float sphere_eval = sphere.eval(x);
	cout << "sphere_eval: " << sphere_eval << endl;
	*/

	return 0;
}

