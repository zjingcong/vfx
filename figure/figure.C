# include "Shape.h"
# include "Vector.h"
# include "Camera.h"
# include "Renderer.h"
# include "VolumeColor.h"
# include "OIIOFiles.h"

# include <iostream>
# include <stdlib.h>

using namespace lux;
using namespace std;

# define STEP_SIZE 0.3
# define WEIGHT 100
# define HEIGHT 100


int main()
{
	// volume positions
	Vector sphere_pos(0.0, 0.0, 0.0);
	// volume setup
	Sphere sphere(sphere_pos, 10.0);
	Color redColor(255.0, 0.0, 0.0, 255.0);
	// camera setup
	Camera myCamera;
	Vector eye(30.0, 0.0, 0.0);
  Vector view(-1.0, 0.0, 0.0);
  Vector up(0.0, 1.0, 0.0);
  myCamera.setEyeViewUp(eye, view, up);
  // renderer setup
  Renderer myRenderer(STEP_SIZE);
	Image myImg;
	myImg.reset(WEIGHT, HEIGHT);
	// rendering
	myRenderer.render(myImg, myCamera, sphere, redColor);
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

