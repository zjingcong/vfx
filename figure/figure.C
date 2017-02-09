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

# define STEP_SIZE 0.5
# define WEIGHT 320
# define HEIGHT 180
# define CAMERA 10
# define NEAR 7
# define FAR 20
# define FRAME_NUM 2
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

Color redColor(1.0, 0.0, 0.0, 0.5);
Color greenColor(0.0, 1.0, 0.0, 0.5);
Color whiteColor(1.0, 1.0, 1.0, 1.0);
ConstantColor red(redColor);
ConstantColor green(greenColor);
ConstantColor white(whiteColor);

ConstantFloat consRho_1(1.0);
ConstantFloat consRho_2(10.0);

Vector originEye(CAMERA, 0.0, 0.0);

// ------------------------------------------------------------------------------------

// volume setup
// uphead
Sphere head1(1.0);
Box box1(10.0, 6);
ScalarTranslate cutoutbox1(box1, Vector(0.0, -1.4, 0.0));
ImplicitCutout uphead(head1, cutoutbox1);
ColorVolume upColor(green, uphead);
DensityVolume upDensity(consRho_1, uphead);
KVolume upK(0.1, uphead);
//lowhead
Box box2(0.8, 6);
ScalarTranslate cutoutbox2(box2, Vector(0.0, -1.6, 0.0));
ImplicitIntersec lowhead1(box2, cutoutbox2);
ScalarTranslate lowhead(lowhead1, Vector(0.0, 0.3, 0.0));
ColorVolume lowColor(green, lowhead);
DensityVolume lowDensity(consRho_1, lowhead);
KVolume lowK(0.1, lowhead);
// left eye
Sphere eye1(0.4);
ScalarTranslate lefteye(eye1, Vector(0.0, 1.0, 0.5));
ColorVolume leftEyeColor(white, lefteye);
DensityVolume leftEyeDensity(consRho_2, lefteye);
KVolume leftEyeK(0.1, lefteye);

// color add
ColorAdd color1(upColor, lowColor);
ColorAdd finalColor(color1, leftEyeColor);

// density add
FloatAdd dens1(upDensity, lowDensity);
FloatAdd finalDensity(dens1, leftEyeDensity);

// K add
FloatAdd k1(upK, lowK);
FloatAdd finalK(k1, leftEyeK);

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
		myCamera.setFarPlane(NEAR);
		myCamera.setFarPlane(FAR);

		float angle = float(180) / FRAME_NUM;
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
		myRenderer.render(finalColor, finalDensity, finalK);

		// write into file
		char file_name[50];
		sprintf(file_name, "./result/jingcoz_hw1.%04d.exr", frame_id);
		cout << "Render frame " << file_name << "..."<< endl;
		writeOIIOImage(file_name, myImg);
	}
	cout << "Rendering complete!" << endl;

	return 0;
}

