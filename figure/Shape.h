//
// basic shape: 
// sphere, box, torus, steiner patch, cone, icosahedron, ellipse, plane
//

# ifndef __LUX_SHAPE_H__
# define __LUX_SHAPE_H__

# include "Volume.h"
# include "Vector.h"
# include "math.h"

# define GOLDEN_RATIO 1.61803399
# define PI 3.14159265


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


	// ellipse(float r_major, float r_minor, Vector normal)
	class Ellipse: public Volume<float>
	{
		public: 
			Ellipse(float r1, float r2, Vector n): R_major(r1), R_minor(r2), normal(n.unitvector())	{}
			~Ellipse()	{}

			const float eval(const Vector& x) const
			{
				float Z = x * normal;
				Vector x_perp = x - Z * normal;

				float a = pow(Z, 2.0) / pow(R_major, 2.0);
				float b = pow(x_perp.magnitude(), 2.0) / pow(R_minor, 2.0);

				return 1 - a - b;
			}

		private:
			float R_major;
			float R_minor;
			Vector normal;
	};


	// icosahedron
	class Icosahedron: public Volume<float>
	{
		public:
			Icosahedron()	{}
			~Icosahedron() {}

			const float eval(const Vector& x) const
			{
				float x_norm = x.magnitude();
				float f;
				if (x_norm > 1.8 * PI)	{f = -1.8 * PI;}
				else
				{
					double xx = x.X();
					double xy = x.Y();
					double xz = x.Z();
					double T = GOLDEN_RATIO;

					f = cos(xx + T * xy) + cos(xx - T * xy) + 
	 				 		cos(xy + T * xz) + cos(xy - T * xz) + 
							cos(xz + T * xx) + cos(xz - T * xx) - 2.0;
				}

				return f;
			}
	};


	// cone(float theta, float h, Vector normal, Vector x0)
	// theta in degree
	class Cone: public Volume<float>
	{
		public:
			Cone(float theta, float h, Vector n, Vector x0): 
				theta(theta), 
				h(h), 
				normal(n.unitvector()), 
				x0(x0)	{}
			~Cone()	{}

			const float eval(const Vector& x) const
			{
				float f;
				float tmp = (x - x0) * normal;
				if (x == x0)	
					{f = 0.0;}
				else if (tmp > h)
					{f = h - tmp;}
				else if (tmp < 0)
					{f = tmp;}
				else
					{f = theta * PI / 180.0 - acos(tmp / (x - x0).magnitude());}

				return f;
			}

		private:
			float theta;
			float h;
			Vector normal;
			Vector x0;
	};


	// plane(Vector normal, Vector x0)
	// default x0: (0.0, 0.0, 0.0)
	// problem need to fix: 
	// when normal(0.0, 1.0, 0.0) with x0 at original point, the result is half plane
	class Plane: public Volume<float>
	{
		public:
			Plane(Vector n, Vector x0 = Vector(0.0, 0.0, 0.0)): x0(x0), normal(n.unitvector())	{}
			~Plane()	{}

			const float eval(const Vector& x) const	{return (x - x0) * normal;}

		private:
			Vector x0;
			Vector normal;
	};

}


#endif

