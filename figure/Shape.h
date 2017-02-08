# ifndef __LUX_SHAPE_H__
# define __LUX_SHAPE_H__

# include "Volume.h"
# include "Vector.h"
# include "math.h"

namespace lux
{

	// sphere(float radius)
	class Sphere: public Volume<float>
	{
		public:
			Sphere(float r): radius(r) {}
			~Sphere() {};

			const float eval(const Vector& x) const {return radius - (x).magnitude();}
		
		private:
			float radius;
	};


	// box(float radius, int param)
	class Box: public Volume<float>
	{
		public:
			Box(float r, int p): radius(r), param(p)	{}
			~Box()	{}

			const float eval(const Vector& x) const	{return radius - pow(x.X(), 2 * param) - pow(x.Y(), 2 * param) - pow(x.Z(), 2 * param);}

		private:
			float radius;
			float param;
	};


	// torus(float r_major, float r_minor, Vector normal)
	// default normal is (1.0, 0.0, 0.0)
	class Torus: public Volume<float>
	{
		public:
			Torus(float r1, float r2, Vector n = Vector(1.0, 0.0, 0.0)): R_major(r1), R_minor(r2), normal(n.unitvector())	{}
			~Torus() 	{}

			const float eval(const Vector& x) const
			{
				Vector x_perp = x - (x * normal) * normal;
				float a = 4 * pow(R_major, 2.0) * pow(x_perp.magnitude(), 2.0);
				float b = pow(x.magnitude(), 2.0) + pow(R_major, 2.0) - pow(R_minor, 2.0);

				return a - pow(b, 2.0);
			}
		
		private:
			float R_major;
			float R_minor;
			Vector normal;
	};


	// steinerpatch
	class SteinerPatch: public Volume<float>
	{
		public:
			SteinerPatch()	{}
			~SteinerPatch()	{}

			const float eval(const Vector& x) const
			{
				float a = pow(x.X(), 2.0) * pow(x.Y(), 2.0);
				float b = pow(x.X(), 2.0) * pow(x.Z(), 2.0);
				float c = pow(x.Y(), 2.0) * pow(x.Z(), 2.0);
				float d = x.X() * x.Y() * x.Z();

				return -(a + b + c - d);
			}
	};

}


#endif

