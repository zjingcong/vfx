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


// rgb range from 0 ~ 255
std::vector<Color> colorList = {
        Color(0.0, 10.0, 255.0, 255.0),
        Color(0.0, 0.0, 255.0, 255.0),
        Color(17.0, 0.0, 255.0, 255.0),
        Color(38.0, 0.0, 255.0, 255.0),
        Color(98.0, 0.0, 255.0, 255.0),
        Color(168.0, 0.0, 255.0, 255.0),
        Color(98.0, 0.0, 255.0, 255.0),
        Color(153.0, 0.0, 255.0, 255.0),
        Color(138.0, 0.0, 255.0, 255.0),
        Color(98.0, 0.0, 255.0, 255.0),
        Color(38.0, 0.0, 255.0, 255.0),
        Color(17.0, 0.0, 255.0, 255.0),
        Color(0.0, 0.0, 255.0, 255.0)
};


void getPyroChainParms()
{
    for (int i = 0; i < 36; ++i)
    {
        float color_scale = 0.5;
        float fjump = drand48() * 1.53 + 2.0382;
        float pscale = drand48() * 0.73 + 0.2;
        float frequency = (drand48() - 0.5) / 8.3 + 0.47235;
        int interval = ceil(36.0 / colorList.size());
        Color color = (colorList.at(int(i / interval))) / 255.0 * color_scale;
        fjumpVecs.push_back(fjump);
        pscaleVecs.push_back(pscale);
        freqVecs.push_back(frequency);
        colorVecs.push_back(color);
    }
}


# endif
