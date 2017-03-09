
# include <iostream>
# include "NoiseGrid.h"

using namespace std;

int main(int argc, char* argv[])
{
    Noise_t parms;
    parms.seed = 485758;
    parms.octaves = 1.0f;
    parms.frequency = 1.0f;
    parms.fjump = 2.0f;
    parms.roughness = 0.5;
    parms.time = 0.0f;

    FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    NoiseGrid perlinNoiseGrid(perlin, 0.01);
    FloatGrid::Ptr perlinGrid = perlinNoiseGrid.getNoiseGrid();

    return 0;
}