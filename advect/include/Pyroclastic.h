# ifndef __PYROCLASTIC_H__
# define __PYROCLASTIC_H__

# include <cmath>
# include <vector>

# include "Types.h"
# include "Shape.h"
# include "Vector.h"
# include "Noise.h"
# include "Grid.h"
# include "VectorField.h"

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


// PyroLevelsets volume
class PyroLevelsets: public Volume<float>
{
    public:
        PyroLevelsets(FloatGridVolume& e, Noise& n, float bg);
        ~PyroLevelsets()    {delete cptFieldPtr;}

        const float eval(const Vector& x) const;

    private:
        FloatGridVolume& levelsetsVolume; // levelsets
        Noise& noise;
        float background;

        Noise_t noiseParm;
        VolumeVectorPtr cptFieldPtr;
        FloatGrid::Ptr levelsetsGrid;
};

# endif
