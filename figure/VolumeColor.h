#ifndef __LUX_COLOR_H__
#define __LUX_COLOR_H__

# include "Color.h"
# include "Volume.h"
# include "Vector.h"

namespace lux
{

  class ColorConstant: public Volume<Color>
  {
    public: 
      ColorConstant(Color color_value): value(color_value)  {};
			~ColorConstant() {};
      
      const Color eval(const Vector& x) const {return value;}

    private:
      Color value;
  };

}

#endif

