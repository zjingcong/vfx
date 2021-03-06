#ifndef __LUX_IMPLICITFUNCOPS_H__
#define __LUX_IMPLICITFUNCOPS_H__

# include "Volume.h"
# include "Vector.h"
# include "math.h"
# include "Color.h"

# define max(x, y) (x > y ? x : y)
# define min(x, y) (x < y ? x : y)


namespace lux
{

	// ============================Float Ops============================================
	class FloatAdd: public Volume<float>
	{
		public:
			FloatAdd(Volume<float>& f, Volume<float>& g): e1(f), e2(g)	{}
			~FloatAdd()	{}

			const float eval(const Vector& x) const	{return e1.eval(x) + e2.eval(x);}

		private:
			Volume<float>& e1;
			Volume<float>& e2;
	};


	// ============================Color Ops============================================
	class ColorAdd: public Volume<Color>
	{
		public:
			ColorAdd(Volume<Color>& f, Volume<Color>& g): e1(f), e2(g)	{}
			~ColorAdd()	{}

			const Color eval(const Vector& x) const	{return e1.eval(x) + e2.eval(x);}

		private:
			Volume<Color>& e1;
			Volume<Color>& e2;
	};

/*
	class ColorAddList: public Volume<Color>
	{
		public:
			ColorAddList(Volume<Color> f[], int num): e1(f), num(num)	{}
			~ColorAddList()	{}

			const Color eval(const Vector& x) const
			{
				Color result;
				for (int i = 0; i < num; ++i)
				{
					result += e1[i].eval(x);
					cout << "******************" << endl;
					cout << result.X() << result.X() << result.X() << endl;
				}
				return result;
			}

		private:
			VolumeColorPtr e1;
			int num;
	};
*/

	// ============================ CSG Ops============================================= 
	// union
	class ImplicitUnion: public Volume<float>
	{
		public:
			ImplicitUnion(Volume<float>& f, Volume<float>& g): e1(f), e2(g) {}
			~ImplicitUnion()	{}

			const float eval(const Vector& x) const	{return max(e1.eval(x), e2.eval(x));}

		private:
			Volume<float>& e1;
			Volume<float>& e2;
	};


	// intersection
	class ImplicitIntersec: public Volume<float>
	{
		public:
			ImplicitIntersec(Volume<float>& f, Volume<float>& g): e1(f), e2(g) {}
			~ImplicitIntersec()	{}

			const float eval(const Vector& x) const	{return min(e1.eval(x), e2.eval(x));}

		private:
			Volume<float>& e1;
			Volume<float>& e2;
	};


	// cutout
	class ImplicitCutout: public Volume<float>
	{
		public:
			ImplicitCutout(Volume<float>& f, Volume<float>& g): e1(f), e2(g) {}
			~ImplicitCutout()	{}

			const float eval(const Vector& x) const	{return min(e1.eval(x), -(e2.eval(x)));}

		private:
			Volume<float>& e1;
			Volume<float>& e2;
	};


	// blinn blend: h = e^(f/s1) + e^(g/s2) - beta
	class ImplicitBlinnBlend: public Volume<float>
	{
		public:
			ImplicitBlinnBlend(Volume<float>& f, Volume<float>& g, float s1 = 1.0, float s2 = 1.0, float beta = 2.0): e1(f), e2(g),  s1(s1), s2(s2), beta(beta){}
			~ImplicitBlinnBlend()	{}

			const float eval(const Vector& x) const	{return exp((e1.eval(x)) / s1) + exp((e2.eval(x)) / s2) - beta;}

		private:
			Volume<float>& e1;
			Volume<float>& e2;
			float s1, s2, beta;
	};


	// mask
	class ImplicitMask: public Volume<float>
	{
		public:
			ImplicitMask(Volume<float>& f): e1(f) {}
			~ImplicitMask()	{}

			const float eval(const Vector& x) const	{return (e1.eval(x) > 0.0) ? 1.0 : 0.0;}

		private:
			Volume<float>& e1;
	};


	// clamp
	class ImplicitClamp: public Volume<float>
	{
		public:
			ImplicitClamp(Volume<float>& f, float a, float b): e1(f), a(a), b(b) {}
			~ImplicitClamp()	{}

			const float eval(const Vector& x) const	
			{
				float f = e1.eval(x);
				if (f <= a)	{return a;}
				if (f >= b)	{return b;}
				return f;
			}

		private:
			Volume<float>& e1;
			float a, b;
	};

}

#endif

