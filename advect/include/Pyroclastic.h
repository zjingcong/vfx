# ifndef __PYROCLASTIC_H__
# define __PYROCLASTIC_H__

# include <cmath>

# include "Shape.h"
# include "Vector.h"
# include "Noise.h"
# include "Types.h"

using namespace lux;


// pyrosphere volume
class Pyrosphere: public Volume<float>
{
    public:
        Pyrosphere(Noise& n);
        ~Pyrosphere()   {}

        const float eval(const Vector& x) const;
        BBox getBBox()  {return pyroBBox;}

    private:
        Noise& noise;

        Noise_t myPyroParam;
        BBox pyroBBox;
};

# endif
