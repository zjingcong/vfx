# ifndef __VECTORFIELD_H__
# define __VECTORFIELD_H__

# include "Types.h"
# include "Volume.h"
# include "Vector.h"
# include "Grid.h"

using namespace lux;


// identity
class Identity: public Volume<Vector>
{
    public:
        Identity()  {}
        ~Identity() {}
        inline const Vector eval(const Vector& x) const {return x;}
};


// vector noise
class VNoise1: public Volume<Vector>
{
    public:
        VNoise1(Noise& n, Vector x, Vector y, Vector z): noise(n), delta_x(x), delta_y(y), delta_z(z)   {}
        ~VNoise1()  {}

        inline const Vector eval(const Vector& x) const
        {
            Noise_t noiseParm = noise.getNoiseParameters();
            double scale = pow( 1.0 + noiseParm.roughness, noiseParm.octaves - 1.0);

            float n0 = (2 * noise.eval(x)) / scale;
            float nx = (2 * noise.eval(x + delta_x)) / scale;
            float ny = (2 * noise.eval(x + delta_y)) / scale;
            float nz = (2 * noise.eval(x + delta_z)) / scale;

            return Vector(nx - n0, ny - n0, nz - n0);
        }

    private:
        Noise& noise;
        Vector delta_x;
        Vector delta_y;
        Vector delta_z;
};


// vector noise
class VNoise2: public Volume<Vector>
{
public:
    VNoise2(Noise& n, Vector x, Vector y, Vector z): noise(n), delta_x(x), delta_y(y), delta_z(z)   {}
    ~VNoise2()  {}

    inline const Vector eval(const Vector& x) const
    {
        float nx = noise.eval(x + delta_x);
        float ny = noise.eval(x + delta_y);
        float nz = noise.eval(x + delta_z);

        return Vector(nx, ny, nz);
    }

private:
    Noise& noise;
    Vector delta_x;
    Vector delta_y;
    Vector delta_z;
};

# endif