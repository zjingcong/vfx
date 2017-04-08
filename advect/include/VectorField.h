# ifndef __VECTORFIELD_H__
# define __VECTORFIELD_H__

# include <openvdb/math/FiniteDifference.h>
# include <openvdb/math/Operators.h>
# include <openvdb/tools/Interpolation.h>

# include "Types.h"
# include "Volume.h"
# include "Vector.h"
# include "Grid.h"

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

using namespace lux;

static int gradErrorNum = 0;

// identity
class Identity: public Volume<Vector>
{
    public:
        Identity()  {}
        ~Identity() {}
        inline const Vector eval(const Vector& x) const {return x;}
};

# endif