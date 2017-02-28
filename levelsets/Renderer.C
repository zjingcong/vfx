
# include "Renderer.h"
# include "Vector.h"
# include "Color.h"
# include "Image.h"
# include "Camera.h"
# include "PropertyVolume.h"
# include "Volume.h"


Color Renderer::rendering(const Vector& x0, const Vector& np, float s_far_near, Volume<float>& densityVolume, Volume<Color>& colorVolume, float K, Volume<Color>& lightVolume)
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
		float rho = densityVolume.eval(x);
		Color c_emission = colorVolume.eval(x);
		Color c_light = lightVolume.eval(x);
		Color color = c_emission + c_light;

    float delta_T = exp(-rho * step_size * K);
   	L += (color / K) * T * (1 - delta_T);
   	T *= delta_T;
		s += step_size;
  }

  return L;
}


s_min_max Renderer::intersect(BBox bbox, const Vector& np)
{
	s_min_max nointer;
	nointer.min = -1;
	nointer.max = -1;
	s_min_max minmax;
	float t0 = camera.nearPlane();
	float t1 = camera.farPlane();
	float xmin, xmax, ymin, ymax, zmin, zmax;
	float smin, smax, symin, symax, szmin, szmax;
	Vec3s xyzmin = bbox.min();
	Vec3s xyzmax = bbox.max();
	xmin = xyzmin.x(); ymin = xyzmin.y(); zmin = xyzmin.z();
	xmax = xyzmax.x(); ymax = xyzmax.y(); zmax = xyzmax.z();
	Vector eye = camera.eye();

	// x component
	if (np.X() >= 0)
	{
		smin = (xmin - eye.X()) / np.X();
		smax = (xmax - eye.X()) / np.X();
	}
	else
	{
		smin = (xmax - eye.X()) / np.X();
		smax = (xmin - eye.X()) / np.X();
	}
	// y component
	if (np.Y() >= 0)
	{
		symin = (ymin - eye.Y()) / np.Y();
		symax = (ymax - eye.Y()) / np.Y();
	}
	else
	{
		symin = (ymax - eye.Y()) / np.Y();
		symax = (ymin - eye.Y()) / np.Y();
	}
	// no intersection
	if ((smin > symax) || (symin > smax))	return nointer;
	if (symin > smin)	smin = symin;
	if (symax < smax)	smax = symax;
	// z component
	if (np.Z() >= 0)
	{
		szmin = (zmin - eye.Z()) / np.Z();
		szmax = (zmax - eye.Z()) / np.Z();
	}
	else
	{
		szmin = (zmax - eye.Z()) / np.Z();
		szmax = (zmin - eye.Z()) / np.Z();
	}
	// no intersection
	if ((smin > szmax) || (szmin > smax))	return nointer;
	if (szmin > smin)	smin = szmin;
	if (szmax < smax)	smax = szmax;
	if ((smin > t1) || (smax < t0))	return nointer;
	if (smin < t0)	smin = t0;
	if (smax > t1)	smax = t1;
	minmax.min = smin;
	minmax.max = smax;

	return minmax;
}


// render with AABB
void Renderer::render(Volume<Color>& colorVolume, Volume<float>& densityVolume, float K, Volume<Color>& lightVolume, BBox volumeBBox)
{
  int width = img.Width();
  int height = img.Height();
  float u, v;
  
	// multithreading
	# pragma omp parallel for collapse(2)
  for (int j = 0; j < height; ++j)
  {
		// # pragma omp parallel for
    for (int i = 0; i < width; ++i)
    {
			v = j / float(height - 1);
      u = i / float(width - 1);
      Vector np = camera.view(u, v);
			
			s_min_max minmax = intersect(volumeBBox, np);
			float s_near = minmax.min;
			float s_far = minmax.max;
      float s_far_near = s_far - s_near;
			Vector x0 = camera.eye() + np * s_near;

      Color L = rendering(x0, np, s_far_near, densityVolume, colorVolume, K, lightVolume);

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

