
# include "Pyroclastic.h"


Pyrosphere::Pyrosphere(Noise &n, float r): noise(n), radius(r)
{
    Vec3s min(-3.5 * r, -3.5 * r, -3.5 * r);
    Vec3s max(3.5 * r, 3.5 * r, 3.5 * r);
    pyroBBox = BBox(min, max);
}


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
