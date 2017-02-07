# ifndef __LUX_SHAPE_H__
# define __LUX_SHAPE_H__

# include "Volume.h"
# include "Shape.h"
# include "Vector.h"

namespace lux
{

	class Sphere: public Volume<float>
	{
		public:
			Sphere(Vector p, float r): pos(p), radius(r) {K = 0;};
			~Sphere() {};

			const float eval(const Vector& x) const {return radius - (x - pos).magnitude();}
      void setK(float k)  {K = k;}
      float getK()  {return K;}
		
		private:
			Vector pos;
			float radius;
      float K;
	};

}


#endif

