// ------------------------------
// Volumes:
// 	- ColorVolume
// 	- DensityVolume
//	- KVolume
// ------------------------------

#ifndef __LUX_PROPERTYVOLUME_H__
#define __LUX_PROPERTYVOLUME_H__

# include "Color.h"
# include "Volume.h"
# include "Vector.h"
# include "Operations.h"
# include "math.h"


namespace lux
{

	// constant float
	class ConstantFloat: public Volume<float>
	{
		public:
			ConstantFloat(float c): constant(c)	{}
			~ConstantFloat()	{}

			const float eval(const Vector& x) const	{return constant;} 

		private:
			float constant;
	};

	// ----------------------------------------------------------------------------

	// constant color
	class ConstantColor: public Volume<Color>
	{
		public:
			ConstantColor(Color color): constantC(color)	{}
			~ConstantColor()	{}

			const Color eval(const Vector& x) const	{return constantC;}

		private:
			Color constantC;
	};


	// color volume for scalar field volume
	class ColorVolume: public Volume<Color>
	{
		public:
			ColorVolume(Volume<Color>& c, Volume<float>& f): colorField(c), e1(f) {}
			~ColorVolume()	{}

			// color mask
			const Color eval(const Vector& x) const	{return ColorMask(x, colorField.eval(x));}

		private:
			Volume<Color>& colorField;
			Volume<float>& e1;

			// use mask for color
			Color ColorMask(const Vector& x, Color c0) const
			{
				ImplicitMask maskedVolume(e1);
				return c0 * (maskedVolume.eval(x));
			}
	};

	// --------------------------------------------------------------------------	
	
	// density for volume
	// clamped: 1 - clamp, 0 - mask
	class DensityVolume: public Volume<float>
	{
		public:
			DensityVolume(Volume<float>& density, Volume<float>& f, int clamped = 1): density(density), e1(f), clamped(clamped)	{}
			~DensityVolume()	{}

			// density mask
			const float eval(const Vector& x) const	
				{return (clamped == 1) ? DensityClamp(x, density.eval(x)) : DensityMask(x, density.eval(x));}

		private:
			Volume<float>& density;
			Volume<float>& e1;
			int clamped;

			// use mask for density
			float DensityMask(const Vector& x, float rho) const
			{
				ImplicitMask maskedVolume(e1);
				return rho * (maskedVolume.eval(x));
			}

			// use clamp for density
			float DensityClamp(const Vector& x, float rho) const
			{
				ImplicitClamp clampedVolume(e1, 0.0, 1.0);
				return rho * (clampedVolume.eval(x));
			}
	};

	// --------------------------------------------------------------------------

	class KVolume: public Volume<float>
	{
		public:
			KVolume(float K, Volume<float>& f): K(K), e1(f)	{}
			~KVolume()	{}

			const float eval(const Vector& x) const	
				{return K * ((e1.eval(x) > 0.0) ? 1.0 : -1.0);}

		private:
			float K;
			Volume<float>& e1;
	};

}

#endif

