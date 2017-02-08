# include "Shape.h"
# include "Vector.h"
# include "Camera.h"
# include "Renderer.h"
# include "OIIOFiles.h"
# include "PropertyVolume.h"
# include "ScalarVolumeXform.h"

# include <iostream>
# include <stdlib.h>

using namespace lux;
using namespace std;

# define STEP_SIZE 0.3
# define WEIGHT 320
# define HEIGHT 180


int main()
{
	// volume setup
	Sphere testVolume1(1.0);
	Vector xt(0.0, 0.5, 0.0);
	ScalarTranslate testVolume(testVolume1, xt);
	// Box testVolume(1.0, 2);
	// Torus testVolume(2.0, 1.0);
	// SteinerPatch testVolume;

	Color redColor(255.0, 0.0, 0.0, 255.0);
	ConstantColor redColorField(redColor);
	ColorVolume sphereColor(redColorField, testVolume);
	ConstantFloat constantDensity(1.0);
	DensityVolume sphereDensity(constantDensity, testVolume);

	// camera setup
	Camera myCamera;
	Vector eye(8.0, 0.0, 0.0);
  Vector view(-1.0, 0.0, 0.0);
  Vector up(0.0, 1.0, 0.0);
  myCamera.setEyeViewUp(eye, view, up);
	myCamera.setFarPlane(10.0);
  // renderer setup
	Image myImg;
	myImg.reset(WEIGHT, HEIGHT);
  Renderer myRenderer(myImg, myCamera, STEP_SIZE);
	// rendering
	myRenderer.render(testVolume, sphereColor, sphereDensity);
	// write into file
	cout << "Write exr image into " << "test.exr" << endl;
	writeOIIOImage("test.exr", myImg);

	return 0;
}

