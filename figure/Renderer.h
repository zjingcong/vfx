# ifndef __LUX_RENDERER_H__
# define __LUX_RENDERER_H__

# include "Vector.h"
# include "Color.h"
# include "Image.h"
# include "Camera.h"
# include "PropertyVolume.h"
# include "Volume.h"

# include "math.h"
# include <vector>

using namespace std;
using namespace lux;

class Renderer
{
	public:
    Renderer(Image& img, Camera& camera, float delta_s): img(img), camera(camera), step_size(delta_s) {}
    ~Renderer() {}

    void render(Volume<float>& scalarVolume, ColorVolume& colorVolume, DensityVolume& densityVolume);

  private:
    float step_size;
		Image& img;
		Camera& camera;

		Color rendering(const Vector& x0, const Vector& np, float s_far_near, DensityVolume& densityVolume, ColorVolume& colorVolume, float K);
};

#endif

