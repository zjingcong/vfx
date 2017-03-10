# ifndef __DOWEDGES_H__
# define __DOWEDGES_H__

# include <iostream>

# include "NoiseGrid.h"
# include "Camera.h"
# include "Color.h"
# include "Grid.h"
# include "Renderer.h"
# include "OIIOFiles.h"

using namespace std;
using namespace lux;


void noiseWedges(VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
                VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float& K)
{
    static Noise_t parms;
    parms.seed = 485758;
    parms.octaves = 1.0f;
    parms.frequency = 1.0f;
    parms.fjump = 2.0f;
    parms.roughness = 0.5;
    parms.time = 0.0f;
    parms.pscale = 5;

    static FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    static NoiseGrid perlinNoiseGrid(perlin, 0.02);
    static FloatGrid::Ptr perlinGrid = perlinNoiseGrid.getNoiseGrid();
    static BBox perlinBBox = perlinNoiseGrid.getNoiseBBox();

    static FloatGridVolume perlinVolume(perlinGrid);

    // -----------------------------------------------------------------------------------
    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantColor perlinMatColor(matColor);
    static ConstantColor perlinEmColor(emColor);


    K = 0.8;
    finalDensityPtr = &perlinVolume;
    finalEmColorPtr = &perlinEmColor;
    finalMatColorPtr = &perlinMatColor;
    finalBBox = perlinBBox;
}

# endif
