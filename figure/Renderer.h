# ifndef __LUX_RENDERER_H__
# define __LUX_RENDERER_H__

# include "Vector.h"
# include "Color.h"
# include "Image.h"
# include "Camera.h"
# include "Shape.h"
# include "VolumeColor.h"

# include "math.h"
# include <vector>

using namespace std;
using namespace lux;

class Renderer
{
	public:
    Renderer(float delta_s): delta_s(delta_s) {};
    ~Renderer() {};

    void render(Image& img, Camera camera, Sphere sphere, Color myColor);

  private:
    float delta_s;
		Color rendering(const Vector& x0, const Vector& np, float s_far_near, float rho, Color color, float K, float delta_s, Sphere sphere);
};

#endif

