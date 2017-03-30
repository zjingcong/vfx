# ifndef __PARMS_H__
# define __PARMS_H__

# include <vector>
# include "Vector.h"

using namespace lux;

std::vector<float> fjumpVecs;
std::vector<float> pscaleVecs;
std::vector<float> freqVecs;


void getPyroChainParms()
{
    for (int i = 0; i < 36; ++i)
    {
        float fjump = drand48() * 1.53 + 2.0382;
        float pscale = drand48() * 0.67 + 0.3;
        float frequency = (drand48() - 0.5) / 8.3 + 0.47235;
        fjumpVecs.push_back(fjump);
        pscaleVecs.push_back(pscale);
        freqVecs.push_back(frequency);
    }
}


# endif
