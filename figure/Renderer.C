# include "Renderer.h"
# include "Vector.h"
# include "Color.h"
# include "Image.h"
# include "Camera.h"
# include "PropertyVolume.h"
# include "Volume.h"


Color Renderer::rendering(const Vector& x0, const Vector& np, float s_far_near, DensityVolume& densityVolume, ColorVolume& colorVolume, float K)
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
    x += np * step_size;
		float rho = densityVolume.DensityMask(x);
		Color color = colorVolume.ColorMask(x);

    float delta_T = exp(-rho * step_size * K);
   	L += (color / K) * T * (1 - delta_T);
   	T *= delta_T;
		s += step_size;
  }

  return L;
}


void Renderer::render(Volume<float>& scalarVolume, ColorVolume& colorVolume, DensityVolume& densityVolume)
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
			
			float s_near = camera.nearPlane();
			float s_far = camera.farPlane();	
      float s_far_near = s_far - s_near;
			Vector x0 = camera.eye() + np * s_near;

			// float rho = 1.0;
      // Color myColor = solidColor.eval(x0);
      float K = 1;

      Color L = Renderer::rendering(x0, np, s_far_near, densityVolume, colorVolume, K);
			// cout << "Color of (" << i << ", " << j << "): " << L.X() << " " << L.Y() << " " << L.Z() << endl;

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

