# ifndef __LUX_SHAPE_H__
# define __LUX_SHAPE_H__

# include "Volume.h"
# include "Vector.h"

namespace lux
{

	class Sphere: public Volume<float>
	{
		public:
			Sphere()	{setup(1.0);}
			Sphere(float r): radius(r) {K = 1;}
			~Sphere() {};

			void setup(float r)	{radius = r;}

			const float eval(const Vector& x) const {return radius - (x).magnitude();}
      void setK(float k)  {K = k;}
      float getK()  {return K;}
		
		private:
			float radius;
      float K;
	};

}


#endif

