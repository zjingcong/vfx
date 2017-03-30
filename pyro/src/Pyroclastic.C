
# include "Pyroclastic.h"


Pyrosphere::Pyrosphere(Noise &n): noise(n)
{
    myPyroParam = noise.getNoiseParameters();
    float r = myPyroParam.pscale;
    Vec3s min(-4 * r, -4 * r, -4 * r);
    Vec3s max(4 * r, 4 * r, 4 * r);
    pyroBBox = BBox(min, max);
}


const float Pyrosphere::eval(const Vector &x) const
{
    float value;
    // create the sphere volume
    float radius = myPyroParam.pscale;
    Sphere sphere(radius);
    float gamma = myPyroParam.gamma;
    Vector xyz = x.unitvector() * radius;
    float noiseValue = pow(fabs(noise.eval(xyz)), gamma);
    value = sphere.eval(x) + noiseValue;

    return value;
}
