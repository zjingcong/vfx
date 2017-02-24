// --------------------------
// Ray Matching Render
// --------------------------


# ifndef __LUX_RENDERER_H__
# define __LUX_RENDERER_H__

# include "Vector.h"
# include "Color.h"
# include "Image.h"
# include "Camera.h"
# include "PropertyVolume.h"
# include "Volume.h"
# include "Types.h"

# include "math.h"
# include <vector>

using namespace std;
using namespace lux;


typedef struct
{
	float min;
	float max;
}	s_min_max;


class Renderer
{
	public:
    Renderer(Image& img, Camera& camera, float delta_s): img(img), camera(camera), step_size(delta_s) {}
    ~Renderer() {}

    void render(Volume<Color>& colorVolume, Volume<float>& densityVolume);

  private:
		Image& img;
		Camera& camera;
		float step_size;

		Color rendering(const Vector& x0, const Vector& np, float s_far_near, Volume<float>& densityVolume, Volume<Color>& colorVolume);

		// smit's method for ray-box intersection
		// return the min and max distance between camera eye and AABB along the view direction
		// if no intersection, return min = -1, max = -1
		s_min_max intersect(BBox bbox, const Vector& np);
};

#endif

