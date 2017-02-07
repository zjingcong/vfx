# include "Renderer.h"
# include "Vector.h"
# include "Color.h"
# include "Image.h"
# include "Camera.h"
# include "Shape.h"
# include "VolumeColor.h"


Color Renderer::rendering(const Vector& x0, const Vector& np, float s_far_near, float rho, Color color, float K, float delta_s, Sphere sphere)
{
  // initialization
  Vector x = x0;
  float T = 1.0;
  Color L;  // L = 0
  float threshold = pow(10.0, -6.0);
  float s = 0;

  // iteration
  while (s <= s_far_near && T >= threshold)
  {
    x += np * delta_s;

		// test
		// rho = (sphere.eval(x) >= 0.0) ? rho : 0.0;
		// cout << "T: " << T << endl;
		// cout << "---------" << endl;
		// if (sphere.eval(x) <= 0.0)	{cout << sphere.eval(x) << endl;	cout << L.X() << ", " << L.Y() << ", " << L.Z() << endl;}
		// cout << "------------------" << endl;
		// test

		// if (sphere.eval(x) <= 0.0)
		// {
    float delta_T = exp(-rho * delta_s * K);
		cout << delta_T << endl;
   	L += (color / K) * T * (1 - delta_T);
   	T *= delta_T;
		// }
  }

	// cout << L.X() << ", " << L.Y() << ", " << L.Z() << endl;

  return L;
}


void Renderer::render(Image& img, Camera camera, Sphere sphere, Color myColor)
{
  int width = img.Width();
  int height = img.Height();
  float u, v;
  
  for (int j = 0; j < height; ++j)
  {
    v = j / float(height - 1);
    for (int i = 0; i < width; ++i)
    {
      u = i / float(width - 1);
      Vector np = camera.view(u, v);
			// cout << "np: " << endl;			
			// cout << np.X() << ", " << np.Y() << ", " << np.Z() << endl;

      // test
      float s_near = 20.0;
      float s_far = 40.0;
      float s_far_near = 20.0;
			float rho = 1.0;
      Vector x0 = camera.eye() + np * s_near;
      // Color myColor = solidColor.eval(x0);
      float K = 1;
      // test
      Color L = Renderer::rendering(x0, np, s_far_near, rho, myColor, K, delta_s, sphere);

      std::vector<float> colorValue;
      colorValue.resize(4);
      colorValue[0] = L.X();
      colorValue[1] = L.Y();
      colorValue[2] = L.Z();
      colorValue[3] = L.W();
      setPixel(img, i, j, colorValue);
    }
  }
}

