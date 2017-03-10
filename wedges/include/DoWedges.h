# ifndef __DOWEDGES_H__
# define __DOWEDGES_H__

# include <iostream>
# include "omp.h"

# include "NoiseGrid.h"
# include "Camera.h"
# include "Color.h"
# include "Grid.h"
# include "Renderer.h"
# include "OIIOFiles.h"
# include "Parms.h"

using namespace std;
using namespace lux;

# define WEIGHT 960
# define HEIGHT 540
# define STEP_SIZE 0.005
# define LIGHT_STEP_SIZE 0.01
# define NEAR 0.1
# define FAR 40


// noise wedges
// fade cannot be even
void createNoise(Noise_t& parms, float fade, VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
                 VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float& K)
{
    static FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    float pscale = parms.pscale;
    float voxelSize = float(pscale * 2) / 500;

    static NoiseGrid perlinNoiseGrid(perlin, voxelSize, fade);
    static FloatGrid::Ptr perlinGrid = perlinNoiseGrid.getNoiseGrid();
    static BBox perlinBBox = perlinNoiseGrid.getNoiseBBox();

    static FloatGridVolume perlinVolume(perlinGrid);

    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantFloat rho(1.0);
    static ConstantColor perlinMatColor(matColor);
    static ConstantColor perlinEmColor(emColor);
    static DensityVolume perlinDensity(rho, perlinVolume);

    K = 0.8;
    // finalDensityPtr = &perlinDensity;
    finalDensityPtr = &perlinVolume;
    finalEmColorPtr = &perlinEmColor;
    finalMatColorPtr = &perlinMatColor;
    finalBBox = perlinBBox;
}


void createNoiseWedges(int frame_id)
{
    cout << "Noise Wedges" << endl;
    string wedge_type = "noise";
    noiseWedgeParms myNoiseParms = noiseParmsList.at(frame_id);

    static Noise_t parms;
    parms.pscale = 5;

    float fade = myNoiseParms.fade;
    parms.octaves = myNoiseParms.octaves;
    parms.frequency = myNoiseParms.freq;
    parms.fjump = myNoiseParms.fjump;

    cout << "=========== Noise Parms ===========" << endl;
    cout << "fade: " << fade << endl;
    cout << "freq: " << parms.frequency << endl;
    cout << "fjump: " << parms.fjump << endl;
    cout << "octaves: " << parms.octaves << endl;
    cout << "===================================" << endl;

    VolumeFloatPtr finalDensityPtr;
    VolumeColorPtr finalEmColorPtr;
    VolumeColorPtr finalMatColorPtr;
    BBox finalBBox;
    float K;

    createNoise(parms, fade, finalDensityPtr, finalEmColorPtr, finalMatColorPtr, finalBBox, K);

    // lighting
    cout << "Set lights..." << endl;
    Lights myLights;
    // light position
    Vector keyPos(0.0, 8.0, 0.0);
    Vector rimPos(0.0, -8.0, 0.0);
    // light color
    Color keyColor(1.0, 1.0, 1.0, 1.0);
    Color rimColor(0.1, 0.1, 0.1, 1.0);
    // set lights
    LightSource keyLight(keyPos, keyColor);
    LightSource rimLight(rimPos, rimColor);
    myLights.push_back(keyLight);
    // myLights.push_back(rimLight);
    LightVolume lightVolume(myLights, *finalDensityPtr, *finalMatColorPtr, K, LIGHT_STEP_SIZE, 0.08, finalBBox);

    cout << "Set image..." << endl;
    Image myImg;
    myImg.reset(WEIGHT, HEIGHT);
    cout << "Set camera..." << endl;
    Camera myCamera;
    Vector eye(20.0, 0.0, 0.0);
    Vector view(-1.0, 0.0, 0.0);
    Vector up(0.0, 1.0, 0.0);
    myCamera.setFarPlane(NEAR);
    myCamera.setFarPlane(FAR);
    myCamera.setEyeViewUp(eye, view, up);
    cout << "Start rendering..." << endl;
    Renderer myRenderer(myImg, myCamera, STEP_SIZE);
    myRenderer.render(*finalEmColorPtr, *finalDensityPtr, K, lightVolume, finalBBox, 1);
    cout << "Rendering complete." << endl;
    // write into file
    char file_name[50];
    sprintf(file_name, "../output/jingcoz_hw3_%s.%04d.exr", wedge_type.c_str(), frame_id);
    cout << "Write frame " << frame_id << " into" << file_name << "."<< endl;
    writeOIIOImage(file_name, myImg);
}

# endif
