#ifndef __LUX_PROPERTYVOLUME_H__
#define __LUX_PROPERTYVOLUME_H__

# include "Color.h"
# include "Volume.h"
# include "Vector.h"
# include "ImplicitFuncOps.h"


namespace lux
{

	class ColorVolume: public Volume<Color>
	{
		public:
			ColorVolume(): e1(tmp)	{}
			// constant color for scalar field volume
			ColorVolume(Volume<float>& f, Color c0): e1(f), constantColor(c0)	{}
			~ColorVolume()	{}

			void setConstantColor(Volume<float>& f, Color c0)	{e1 = f; constantColor = c0;}

			// use mask for color
			Color ColorMask(const Vector& x) const
			{
				ImplicitMask maskedVolume(e1);
				return constantColor * (maskedVolume.eval(x));
			}

		private:
			Volume<float> tmp;
			Volume<float>& e1;
			Color constantColor;	
	};

	// --------------------------------------------------------------------------	
	
	// density for volume
	class DensityVolume: public Volume<float>
	{
		public:
			DensityVolume(): e1(tmp), density(1.0)	{}
			// constant density for scalar field volume
			DensityVolume(Volume<float>& f, float density): e1(f), density(density)	{}
			~DensityVolume()	{}

			void setConstantDensity(Volume<float>& f, float d)	{e1 = f; density = d;}

			// use mask for density
			float DensityMask(const Vector& x) const
			{
				ImplicitMask maskedVolume(e1);
				return density * (maskedVolume.eval(x));
			}

			// use clamp for density
			float DensityClamp(const Vector& x)
			{
				ImplicitClamp clampedVolume(e1, 0.0, 1.0);
				return density * (clampedVolume.eval(x));
			}

		private:
			Volume<float> tmp;
			Volume<float>& e1;
			float density;
	};
}

#endif

