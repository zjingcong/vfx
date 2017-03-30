# ifndef __PARMS_H__
# define __PARMS_H__

# include <vector>
# include "Vector.h"
# include "Color.h"

using namespace lux;

std::vector<float> fjumpVecs;
std::vector<float> pscaleVecs;
std::vector<float> freqVecs;
std::vector<Color> colorVecs;


// rgb range from 0 ~ 1
std::vector<Color> colorList = {
        Color(0.047, 1.0, 0.89, 1.0),
        Color(0.341, 1.0, 0.815, 1.0),
        Color(0.235, 1.0, 0.725, 1.0),
        Color(0.0, 0.584, 1.0, 1.0),
        Color(0.0196, 0.8, 0.58, 1.0)
};

void getPyroChainParms()
{
    for (int i = 0; i < 36; ++i)
    {
        float color_scale = 0.5;
        float fjump = drand48() * 1.53 + 2.0382;
        float pscale = drand48() * 0.67 + 0.3;
        float frequency = (drand48() - 0.5) / 8.3 + 0.47235;
        Color color = (colorList.at(int(drand48() * 5))) * color_scale;
        fjumpVecs.push_back(fjump);
        pscaleVecs.push_back(pscale);
        freqVecs.push_back(frequency);
        colorVecs.push_back(color);
    }
}


# endif
