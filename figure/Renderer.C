# include "Renderer.h"
# include "Vector.h"
# include "Color.h"
# include "Image.h"
# include "Camera.h"
# include "Shape.h"
# include "VolumeColor.h"

Color Renderer::rendering(const Vector& x0, const Vector& np, float s_far_near, float rho, Color color, float K, float delta_s)
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
    float delta_T = exp(-rho * delta_s * K);
    L += (color / K) * T * (1 - delta_T);
    T *= delta_T;
  }

  return L;
}


void Renderer::render(Image img, Camera camera, Sphere sphere)
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

      // test
      float s_near = 10.0;
      float s_far = 30.0;
      float s_far_near = 20.0;
      float rho = 1.0;
      Vector x0 = camera.eye() + np * s_near;
      Color myColor;
      myColor.set(255.0, 255.0, 255.0, 255.0);
      float K = 1;
      // test
      Color L = Renderer::rendering(x0, np, s_far_near, rho, myColor, K, delta_s);

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

