# ifndef __PYROCLASTIC_H__
# define __PYROCLASTIC_H__

# include <cmath>

# include "Shape.h"
# include "Vector.h"

using namespace lux;


class Pyrosphere: public Volume<float>
{
    public:
        Pyrosphere(Noise& n, float r): noise(n), radius(r)  {}
        ~Pyrosphere()   {}

        const float eval(const Vector& x) const;

    private:
        Noise& noise;
        float radius;
};


const float Pyrosphere::eval(const Vector &x) const
{
    // create the sphere volume
    Sphere sphere(radius);
    float value;
    Noise_t myPyroParam = noise.getNoiseParameters();
    float gamma = myPyroParam.gamma;
    Vector xyz = x.unitvector() * radius;
    float noiseValue = pow(fabs(noise.eval(xyz)), gamma);
    value = sphere.eval(x) + noiseValue;

    return value;
}

# endif
