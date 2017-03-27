// ---------------------------------
// ScalarVolume Transformation:
// 	- Translate
// 	- Scaling
// 	- Rotation
// ---------------------------------

#ifndef __LUX_SCALARVOLUMEXFORM_H__
#define __LUX_SCALARVOLUMEXFORM_H__

# include "Volume.h"
# include "Vector.h"
# include "math.h"

# define PI 3.14159265

using namespace std;


namespace lux
{

	// translate
	class ScalarTranslate: public Volume<float>
	{
		public:
			ScalarTranslate(Volume<float>& f, Vector xt): e1(f), xt(xt)	{}
			~ScalarTranslate() {}

			inline const float eval(const Vector& x) const	{return e1.eval(x - xt);}

		private:
			Volume<float>& e1;
			Vector xt;
	};


	// scaling
	class ScalarScaling: public Volume<float>
	{
		public:
			ScalarScaling(Volume<float>& f, float lambda): e1(f), lambda(lambda)	{}
			~ScalarScaling()	{}

			inline const float eval(const Vector& x) const	{return e1.eval(x / float(lambda));}

		private:
			Volume<float>& e1;
			float lambda;
	};


	// rotation
	// theta in degree
	class ScalarRotation: public Volume<float>
	{
		public:
			ScalarRotation(Volume<float>& f, Vector n, float theta): e1(f), axis(n.unitvector()), theta(theta)	{}
			~ScalarRotation()	{}

			inline const float eval(const Vector& x) const
			{
				Vector Vr;
				// f'(x) = f(R(n, -theta)x)
				// -theta
				double cos_theta = cos((-theta) * PI / 180.0);
				double sin_theta = sin((-theta) * PI / 180.0);
				// multipilcation: *, inner product: *, cross product: ^
				Vr = x * cos_theta + (axis * x) * (1 - cos_theta) * axis + (axis ^ x) * sin_theta;
				
				return e1.eval(Vr);
			}

		private:
			Volume<float>& e1;
			Vector axis;
			float theta;
	};

}

#endif

