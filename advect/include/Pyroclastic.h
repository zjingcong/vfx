# ifndef __PYROCLASTIC_H__
# define __PYROCLASTIC_H__

# include <cmath>
# include "math.h"
# include <vector>
# include <openvdb/math/FiniteDifference.h>
# include <openvdb/math/Operators.h>

# include "Types.h"
# include "Shape.h"
# include "Vector.h"
# include "Noise.h"
# include "Grid.h"
# include "VectorField.h"
# include "Levelsets.h"

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


// generate pyroclast volume from openVDB levelsets
class PyroVDBLevelsets: public Volume<float>
{
    public:
        PyroVDBLevelsets(VDBLevelsetsPtr l, Noise& n);
        ~PyroVDBLevelsets() {delete levelsetsVolumePtr;}

        const float eval(const Vector& x) const;

    private:
        VDBLevelsetsPtr levelsetsGrid;
        Noise& noise;

        VolumeFloatPtr levelsetsVolumePtr;
        Noise_t noiseParm;
};

# endif
