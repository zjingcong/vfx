# ifndef __RANDOMM_H__
# define __RANDOMM_H__

# include <cmath>
# include "Vector.h"


namespace randm
{

    inline float phi(int b, int i) {
        float x, f;
        x = 0.0;
        f = 1.0 / float(b);
        while (i) {
            x += f * (float(i % b));
            i /= b;
            f *= 1.0 / float(b);
        }

        return x;
    }


    inline lux::Vector hemisphereRand(int id) {
        float az = 2 * M_PI * phi(2, id);
        float el = asin(phi(3, id));
        float x = -sin(az) * cos(el);
        float y = sin(el);
        float z = cos(az) * cos(el);

        return lux::Vector(x, y, z);
    }

}

# endif
