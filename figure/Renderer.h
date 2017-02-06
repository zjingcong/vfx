# ifndef __LUX_RENDERER_H__
# define __LUX_RENDERER_H__

# include "Vector.h"
# include "Color.h"

namespace lux
{

class Renderer
{
	public: 
		float rendering(const Vector& x0, const Vector& np, float s, float rho, Color color, float K, float delta_s);
};

Renderer::rendering(const Vector& x0, const Vector& np, float s, float rho, Color color, float K, float delta_s)
{
}

}

#endif

