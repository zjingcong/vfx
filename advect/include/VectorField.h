# ifndef __VECTORFIELD_H__
# define __VECTORFIELD_H__

# include "Types.h"
# include "Volume.h"
# include "Vector.h"
# include "Grid.h"

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

using namespace lux;


// identity
class Identity: public Volume<Vector>
{
    public:
        Identity()  {}
        ~Identity() {}
        inline const Vector eval(const Vector& x) const {return x;}
};


// closest point transform
class LevelsetsCPT: public Volume<Vector>
{
    public:
        LevelsetsCPT(FloatGridVolume& e): sdf(e) {}
        ~LevelsetsCPT()  {}

        const Vector eval(const Vector& x) const
        {
            float value = sdf.eval(x);
            Vector grad = sdf.grad(x);

//            if (grad.magnitude() < 0.7)
//            {std::cout << "grad: " << grad.magnitude() << " value: " << value << " x: " << x.X() << " " << x.Y() << " " << x.Z() << std::endl;}

            return x - value * grad;
        }

    private:
        FloatGridVolume& sdf; // signed distance function
};

# endif