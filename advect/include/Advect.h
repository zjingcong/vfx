# ifndef __ADVECT_H__
# define __ADVECT_H__

# include "Volume.h"
# include "Vector.h"

using namespace lux;


class VectorAdvect: public Volume<Vector>
{
    public:
        VectorAdvect(VolumeVectorPtr vec, VolumeVectorPtr v, float t): vectorField(vec), velocityField(v), time_step(t) {}
        ~VectorAdvect() {}

        inline const Vector eval(const Vector& x) const
        {
            Vector vec = vectorField->eval(x);
            Vector v = velocityField->eval(x);

            return vec - v * time_step;
        }

    private:
        VolumeVectorPtr vectorField;
        VolumeVectorPtr velocityField;
        float time_step;
};

typedef VectorAdvect* VectorAdvectPtr;


class Warp: public Volume<float>
{
    public:
        Warp(VolumeFloatPtr d, VolumeVectorPtr v): densityVolumePtr(d), vecAdvect(v)  {}
        ~Warp() {}

        inline const float eval(const Vector& x) const
        {
            Vector vec = vecAdvect->eval(x);
            return densityVolumePtr->eval(vec);
        }

    private:
        VolumeFloatPtr densityVolumePtr;
        VolumeVectorPtr vecAdvect;
};

# endif
