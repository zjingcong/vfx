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
# include <math.h>
# include <stdio.h>

using namespace lux;
using namespace std;

# define STEP_SIZE 0.3
# define WEIGHT 320
# define HEIGHT 180
# define FAR 10
# define FRAME_NUM 4
# define PI 3.14159265


// theta in degree
// axis should be unit vector
Vector vecRotation(Vector v, Vector axis, float a)
{
	float theta = PI * a / 180.0;
	return v * cos(theta) + axis * (axis * v) * ( 1 - cos(theta)) + (axis ^ v) * sin(theta);
}


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


// ===================================== setup ========================================
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

Vector originEye(10.0, 0.0, 0.0);

// ------------------------------------------------------------------------------------


int main()
{
	cout << "Rendering start..." << endl;
	# pragma omp parallel for
	for (int frame_id = 0; frame_id < FRAME_NUM; ++frame_id)
	{
		Image myImg;
		myImg.reset(WEIGHT, HEIGHT);

		Camera myCamera;
		myCamera.setFarPlane(FAR);

		float angle = float(360) / FRAME_NUM;
		float theta = frame_id  * angle;
		Vector up(0.0, 1.0, 0.0);
		Vector lookAt(0.0, 0.0, 0.0);
		Vector camera_axis(0.0, 1.0, 0.0);

		// get camera position and camera view
		Vector eye_new = vecRotation(originEye, camera_axis, theta);
		Vector view_new = lookAt - eye_new;
		myCamera.setEyeViewUp(eye_new, view_new, up);

		// rendering
		Renderer myRenderer(myImg, myCamera, STEP_SIZE);
		myRenderer.render(finalVolume, finalColor, finalDensity);

		// write into file
		char file_name[50];
		sprintf(file_name, "./result/jingcoz_hw1.%04d.exr", frame_id);
		cout << "Render frame " << file_name << "..."<< endl;
		writeOIIOImage(file_name, myImg);
	}
	cout << "Rendering complete!" << endl;

	return 0;
}

