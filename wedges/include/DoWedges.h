# ifndef __DOWEDGES_H__
# define __DOWEDGES_H__

# include <iostream>
# include "omp.h"

# include "NoiseGrid.h"
# include "Pyroclastic.h"
# include "Wisp.h"
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
# define NEAR 0.1
# define FAR 40
# define CRAZY_NUM 2.0316578

float step_size;


/// ----------------------------------- noise wedges ---------------------------------------------------------

// fade cannot be even
void createNoise(Noise_t& parms, float fade, VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
                 VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float& K)
{
    static FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    float pscale = parms.pscale;
    float voxelSize = float(pscale * 2) / 500;
    step_size = float(voxelSize) / CRAZY_NUM;   // get step size form grid

    static NoiseGrid perlinNoiseGrid(perlin, voxelSize, fade);
    static FloatGrid::Ptr perlinGrid = perlinNoiseGrid.getNoiseGrid();
    static BBox perlinBBox = perlinNoiseGrid.getNoiseBBox();

    static FloatGridVolume perlinVolume(perlinGrid);

    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantFloat rho(0.8);
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


void createNoiseWedges(int frame_id, string output_path)
{
    cout << "frame_id: " << frame_id << endl;
    string wedge_type = "noise";
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw3_%s.%04d.exr", output_path.c_str(), wedge_type.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;

    noiseWedgeParms myNoiseParms = noiseParmsList.at(frame_id);

    static Noise_t parms;
    parms.pscale = 6;

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
    float light_voxelSize = 0.08;
    float light_step_size = float(light_voxelSize) / CRAZY_NUM;   // get step size form grid
    cout << "	 | Light voxel size: " << light_voxelSize << endl;
    cout << "	 | Light step size: " << light_step_size << endl;
    LightVolume lightVolume(myLights, *finalDensityPtr, *finalMatColorPtr, K, light_step_size, light_voxelSize, finalBBox);

    cout << "Set image..." << endl;
    Image myImg;
    myImg.reset(WEIGHT, HEIGHT);
    cout << "Set camera..." << endl;
    Camera myCamera;
    Vector eye(24.0, 0.0, 0.0);
    Vector view(-1.0, 0.0, 0.0);
    Vector up(0.0, 1.0, 0.0);
    myCamera.setFarPlane(NEAR);
    myCamera.setFarPlane(FAR);
    myCamera.setEyeViewUp(eye, view, up);
    cout << "Start rendering..." << endl;
    cout << "	 | Render step size: " << step_size << endl;
    Renderer myRenderer(myImg, myCamera, step_size);
    myRenderer.render(*finalEmColorPtr, *finalDensityPtr, K, lightVolume, finalBBox, 1);
    cout << "Rendering complete." << endl;
    // write into file
    cout << "Write frame " << frame_id << " into" << file_name << "."<< endl;
    writeOIIOImage(file_name, myImg);
}


/// ----------------------------------- pyroclastic wedges ---------------------------------------------------

void createPyro(Noise_t& parms, VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
                VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float& K)
{
    static FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    float pyroshpereRadius = 1.0;
    static Pyrosphere myPyrosphere(perlin, 1.0);
    static BBox pyroBBox = myPyrosphere.getBBox();

    float voxelSize = float(pyroBBox.max().x() - pyroBBox.min().x()) / 1000;
    step_size = float(voxelSize) / CRAZY_NUM;   // get step size form grid

    // stamp
    cout << "Stamping pyrosphere..." << endl;
    static FloatVolumeToGrid pyroSphereVolume2Grid(myPyrosphere, voxelSize, pyroBBox);
    static FloatGrid::Ptr pyroSphereGrid = pyroSphereVolume2Grid.getVolumeGrid();
    static BBox pyroNewBBox = pyroSphereVolume2Grid.getBBox();
    // gridded
    static FloatGridVolume pyroSphereVolume(pyroSphereGrid);

    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantFloat rho(5.0);
    static ConstantColor pyroMatColor(matColor);
    static ConstantColor pyroEmColor(emColor);
    static DensityVolume pyroDensity(rho, pyroSphereVolume);

    K = 5;
    finalDensityPtr = &pyroDensity;
    finalEmColorPtr = &pyroEmColor;
    finalMatColorPtr = &pyroMatColor;
    finalBBox = pyroNewBBox;
}


void createPyroWedges(int frame_id, string output_path)
{
    string wedge_type = "pyro";
    cout << "frame_id: " << frame_id << endl;
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw3_%s.%04d.exr", output_path.c_str(), wedge_type.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;

    pyroWedgeParms myPyroParms = pyroParmsList.at(frame_id);

    static Noise_t parms;
    parms.gamma = myPyroParms.gamma;
    parms.fjump = myPyroParms.fjump;
    parms.frequency = myPyroParms.freq;
    parms.octaves = myPyroParms.octaves;

    cout << "=========== Pyro Parms ===========" << endl;
    cout << "octaves: " << parms.octaves << endl;
    cout << "freq: " << parms.frequency << endl;
    cout << "fjump: " << parms.fjump << endl;
    cout << "gamma: " << parms.gamma << endl;
    cout << "==================================" << endl;

    VolumeFloatPtr finalDensityPtr;
    VolumeColorPtr finalEmColorPtr;
    VolumeColorPtr finalMatColorPtr;
    BBox finalBBox;
    float K;

    createPyro(parms, finalDensityPtr, finalEmColorPtr, finalMatColorPtr, finalBBox, K);

    // lighting
    cout << "Set lights..." << endl;
    Lights myLights;
    // light position
    Vector keyPos(0.0, 4.0, 0.0);
    Vector rimPos(0.0, -4.0, 0.0);
    // light color
    Color keyColor(5.0, 5.0, 5.0, 1.0);
    Color rimColor(0.8, 0.8, 0.8, 1.0);
    // set lights
    LightSource keyLight(keyPos, keyColor);
    LightSource rimLight(rimPos, rimColor);
    myLights.push_back(keyLight);
    myLights.push_back(rimLight);
    float light_voxelSize = 0.024;
    float light_step_size = float(light_voxelSize) / CRAZY_NUM;   // get step size form grid
    cout << "	 | Light voxel size: " << light_voxelSize << endl;
    cout << "	 | Light step size: " << light_step_size << endl;
    LightVolume lightVolume(myLights, *finalDensityPtr, *finalMatColorPtr, K, light_step_size, light_voxelSize, finalBBox);

    cout << "Set image..." << endl;
    Image myImg;
    myImg.reset(WEIGHT, HEIGHT);
    cout << "Set camera..." << endl;
    Camera myCamera;
    Vector eye(8.0, 0.0, 0.0);
    Vector view(-1.0, 0.0, 0.0);
    Vector up(0.0, 1.0, 0.0);
    myCamera.setFarPlane(NEAR);
    myCamera.setFarPlane(FAR);
    myCamera.setEyeViewUp(eye, view, up);
    cout << "Start rendering..." << endl;
    cout << "	 | Render step size: " << step_size << endl;
    Renderer myRenderer(myImg, myCamera, step_size);
    myRenderer.render(*finalEmColorPtr, *finalDensityPtr, K, lightVolume, finalBBox, 1);
    cout << "Rendering complete." << endl;
    // write into file
    cout << "Write frame " << frame_id << " into" << file_name << "."<< endl;
    writeOIIOImage(file_name, myImg);
}


/// ------------------------------------- wisp wedges -------------------------------------------------------

void createWisp(wispWedgeParms& wispparms, VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
                VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float& K)
{
    static FractalSum<PerlinNoiseGustavson> FSPN1;
    static FractalSum<PerlinNoiseGustavson> FSPN2;

    static Noise_t FSPN1Parms;
    FSPN1Parms.octaves = wispparms.octaves;
    FSPN1Parms.frequency = wispparms.freq;
    FSPN1Parms.fjump = wispparms.fjump;
    // FSPN1Parms.roughness = 0.1;

    static Noise_t FSPN2Parms;
    FSPN1.setParameters(FSPN1Parms);
    FSPN2.setParameters(FSPN2Parms);

    static WispParms wispParameters;
    wispParameters.clump = wispparms.clump;
    wispParameters.FSPN2 = &FSPN2;

    cout << "Create wisp grid..." << endl;
    static SingleGuideWisp wisp(FSPN1, wispParameters, 0.01, 5000000);
    static FloatGrid::Ptr wispGrid = wisp.getWispGrid();
    static BBox wispBBox = wisp.getBBox();
    cout << "	 | Wisp bounding box: " << wispBBox.min() << " " << wispBBox.max() << endl;
    static FloatGridVolume wispVolume(wispGrid);
    step_size = 0.003;

    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantFloat rho(0.8);
    static ConstantColor wispMatColor(matColor);
    static ConstantColor wispEmColor(emColor);
    static DensityVolume wispDensity(rho, wispVolume);

    K = 0.8;
    // finalDensityPtr = &wispDensity;
    finalDensityPtr = &wispVolume;
    finalEmColorPtr = &wispEmColor;
    finalMatColorPtr = &wispMatColor;
    finalBBox = wispBBox;
}


void createWispWedges(int frame_id, string output_path)
{
    string wedge_type = "wisp";
    cout << "frame_id: " << frame_id << endl;
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw3_%s.%04d.exr", output_path.c_str(), wedge_type.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;

    wispWedgeParms myWispParms = wispParmsList.at(frame_id);

    cout << "=========== Wisp Parms ===========" << endl;
    cout << "octaves: " << myWispParms.octaves << endl;
    cout << "freq: " << myWispParms.freq << endl;
    cout << "fjump: " << myWispParms.fjump << endl;
    cout << "clump: " << myWispParms.clump << endl;
    cout << "==================================" << endl;

    VolumeFloatPtr finalDensityPtr;
    VolumeColorPtr finalEmColorPtr;
    VolumeColorPtr finalMatColorPtr;
    BBox finalBBox;
    float K;

    createWisp(myWispParms, finalDensityPtr, finalEmColorPtr, finalMatColorPtr, finalBBox, K);

    // lighting
    cout << "Set lights..." << endl;
    Lights myLights;
    // light position
    Vector keyPos(0.0, 4.0, 0.0);
    Vector rimPos(0.0, -4.0, 0.0);
    // light color
    Color keyColor(5.0, 5.0, 5.0, 1.0);
    Color rimColor(0.8, 0.8, 0.8, 1.0);
    // set lights
    LightSource keyLight(keyPos, keyColor);
    LightSource rimLight(rimPos, rimColor);
    myLights.push_back(keyLight);
    myLights.push_back(rimLight);
    float light_voxelSize = 0.08;
    float light_step_size = float(light_voxelSize) / CRAZY_NUM;   // get step size form grid
    cout << "	 | Light voxel size: " << light_voxelSize << endl;
    cout << "	 | Light step size: " << light_step_size << endl;
    LightVolume lightVolume(myLights, *finalDensityPtr, *finalMatColorPtr, K, light_step_size, light_voxelSize, finalBBox);

    cout << "Set image..." << endl;
    Image myImg;
    myImg.reset(WEIGHT, HEIGHT);
    cout << "Set camera..." << endl;
    Camera myCamera;
    Vector eye(8.0, 0.0, 0.0);
    Vector view(-1.0, 0.0, 0.0);
    Vector up(0.0, 1.0, 0.0);
    myCamera.setFarPlane(NEAR);
    myCamera.setFarPlane(FAR);
    myCamera.setEyeViewUp(eye, view, up);
    cout << "Start rendering..." << endl;
    cout << "	 | Render step size: " << step_size << endl;
    Renderer myRenderer(myImg, myCamera, step_size);
    myRenderer.render(*finalEmColorPtr, *finalDensityPtr, K, lightVolume, finalBBox, 1);
    cout << "Rendering complete." << endl;
    // write into file
    cout << "Write frame " << frame_id << " into" << file_name << "."<< endl;
    writeOIIOImage(file_name, myImg);
}

# endif
