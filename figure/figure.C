# include "Shape.h"
# include "Vector.h"
# include "Camera.h"
# include "Renderer.h"
# include <iostream>
# include <stdlib.h>

using namespace lux;
using namespace std;


int main()
{
	// volume positions
	Vector sphere_pos(0.0, 0.0, 0.0);
	// volume setup
	Sphere sphere(sphere_pos, 10.0);
	// camera setup
	Camera myCamera();
	

	Vector x(5.0, 0.0, 0.0);
	float sphere_eval = sphere.eval(x);
	cout << "sphere_eval: " << sphere_eval << endl;

	return 0;
}

