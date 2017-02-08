#ifndef __LUX_PROPERTYVOLUME_H__
#define __LUX_PROPERTYVOLUME_H__

# include "Color.h"
# include "Volume.h"
# include "Vector.h"
# include "ImplicitFuncOps.h"


namespace lux
{
	// color for volume
  class ColorConstant: public Volume<Color>
  {
    public: 
      ColorConstant(Color color_value): value(color_value)  {};
			~ColorConstant() {};
      
      const Color eval(const Vector& x) const {return value;}

    private:
      Color value;
  };


	class ColorVolume: public Volume<Color>
	{
		public:
			// constant color for scalar field volume
			ColorVolume(Volume<float>& f, Color c0): e1(f), constantColor(c0)	{}
			~ColorVolume()	{}

			// use mask for color
			Color ColorMask(const Vector& x) const
			{
				ImplicitMask maskedVolume(e1);

				return constantColor * (maskedVolume.eval(x));
			}

		private:
			Volume<float>& e1;
			Color constantColor;	
	};

	// --------------------------------------------------------------------------	
	
	// density for volume
	class DensityVolume: public Volume<float>
	{
		public:
			// constant density for scalar field volume
			DensityVolume(Volume<float>& f, float density): e1(f), density(density)	{}
			~DensityVolume()	{}

			// use mask for density
			float DensityMask(const Vector& x) const
			{
				ImplicitMask maskedVolume(e1);

				return density * (maskedVolume.eval(x));
			}

		private:
			Volume<float>& e1;
			float density;
	};
}

#endif

