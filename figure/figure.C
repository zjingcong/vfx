# include "Shape.h"
# include "Vector.h"
# include "Camera.h"
# include "Renderer.h"
# include "OIIOFiles.h"
# include "PropertyVolume.h"
# include "ScalarVolumeXform.h"
# include "Operations.h"

# include <iostream>
# include <stdlib.h>

using namespace lux;
using namespace std;

# define STEP_SIZE 0.3
# define WEIGHT 320
# define HEIGHT 180


// reference: basicShape & Xform
// ------------------------------------------------------------------------------
	// Sphere testVolume(1.0);
	// Box testVolume1(0.3, 2);
	// Torus testVolume(2.0, 1.0);
	// SteinerPatch testVolume;
	// Ellipse testVolume(3.0, 1.0, Vector(0.0, 1.0, 0.0));
	// Icosahedron testVolume;
	// Cone testVolume3(45.0, 1.0, Vector(0.0, 1.0, 0.0), Vector(0.0, 1.0, 0.0));

// ------------------------------------------------------------------------------
	// ScalarTranslate testVolume2(testVolume1, Vector(0.0, 0.5, 0.0));
	// ScalarRotation testVolume(testVolume1, Vector(1.0, 0.0, 0.0), 45.0);
	// ScalarScaling testVolume(testVolume1, 0.5);
	// ImplicitBlinnBlend testVolume(testVolume2, testVolume3);


int main()
{
	// volume setup
	Sphere volume1(1.0);
	Box box(2.0, 2);
	ScalarTranslate volume2(box, Vector(0.0, 0.0, 2.0));

	Color redColor(1.0, 0.0, 0.0, 0.5);
	Color greenColor(0.0, 1.0, 0.0, 0.5);
	ConstantColor redColorField(redColor);
	ConstantColor greenColorField(greenColor);
	ConstantFloat constantDensity(1.0);

	ImplicitUnion finalVolume(volume1, volume2);
	ColorVolume volume1Color(redColorField, volume1);
	DensityVolume volume1Density(constantDensity, volume1);
	ColorVolume volume2Color(greenColorField, volume2);
	DensityVolume volume2Density(constantDensity, volume2);
	ColorAdd finalColor(volume1Color, volume2Color);
	FloatAdd finalDensity(volume1Density, volume2Density);

	// camera setup
	Camera myCamera;
	Vector eye(10.0, 0.0, 0.0);
	Vector view(-1.0, 0.0, 0.0);
	Vector up(0.0, 1.0, 0.0);
	myCamera.setEyeViewUp(eye, view, up);
	myCamera.setFarPlane(20.0);
  // renderer setup
	Image myImg;
	myImg.reset(WEIGHT, HEIGHT);
  Renderer myRenderer(myImg, myCamera, STEP_SIZE);
	// rendering
	myRenderer.render(finalVolume, finalColor, finalDensity);
	// write into file
	cout << "Write exr image into " << "test.exr" << endl;
	writeOIIOImage("test.exr", myImg);

	return 0;
}

