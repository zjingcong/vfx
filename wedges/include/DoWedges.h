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
# define LIGHT_GRID_NUM_1 250
# define LIGHT_GRID_NUM_2 100
# define LIGHT_GRID_NUM_3 100

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

float step_size;


/// ----------------------------------- noise wedges ---------------------------------------------------------

// fade cannot be even
void createNoise(Noise_t& parms, VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
                 VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float& K)
{
    static FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    float pscale = parms.pscale;
    float fade = parms.falloff;

    float voxelSize = float(pscale * 2) / 500;
    step_size = float(voxelSize) / CRAZY_NUM;   // get step size form grid

    static NoiseGrid perlinNoiseGrid(perlin, voxelSize, fade);
    static FloatGrid::Ptr perlinGrid = perlinNoiseGrid.getNoiseGrid();
    static BBox perlinBBox = perlinNoiseGrid.getNoiseBBox();

    static FloatGridVolume perlinVolume(perlinGrid);

    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantFloat rho(2);
    static ConstantColor perlinMatColor(matColor);
    static ConstantColor perlinEmColor(emColor);
    static DensityVolume perlinDensity(rho, perlinVolume);

    K = 1.5;
    finalDensityPtr = &perlinDensity;
    // finalDensityPtr = &perlinVolume;
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
    parms.pscale = 4.5;
    parms.roughness = 0.4;

    // float fade = myNoiseParms.fade;
    parms.octaves = myNoiseParms.octaves;
    parms.frequency = myNoiseParms.freq;
    parms.fjump = myNoiseParms.fjump;
    parms.falloff = myNoiseParms.fade;

    cout << "=========== Noise Parms ===========" << endl;
    cout << "fade: " << parms.falloff << endl;
    cout << "freq: " << parms.frequency << endl;
    cout << "fjump: " << parms.fjump << endl;
    cout << "octaves: " << parms.octaves << endl;
    cout << "===================================" << endl;

    VolumeFloatPtr finalDensityPtr;
    VolumeColorPtr finalEmColorPtr;
    VolumeColorPtr finalMatColorPtr;
    BBox finalBBox;
    float K;

    createNoise(parms, finalDensityPtr, finalEmColorPtr, finalMatColorPtr, finalBBox, K);

    // lighting
    cout << "Set lights..." << endl;
    Lights myLights;
    // light position
    Vector keyPos(0.0, 8.0, 0.0);
    Vector rimPos(0.0, -8.0, 0.0);
    // light color
    // Color keyColor(1.0, 1.0, 1.0, 1.0);
    // Color rimColor(0.1, 0.1, 0.1, 1.0);
    Color keyColor(0.5, 0.05, 0.39, 1.0);
    Color rimColor(0.04, 0.28, 0.4, 1.0);
    // set lights
    LightSource keyLight(keyPos, keyColor);
    LightSource rimLight(rimPos, rimColor);
    myLights.push_back(keyLight);
    myLights.push_back(rimLight);
    float light_voxelSize = float(parms.pscale) / LIGHT_GRID_NUM_3;
    float light_step_size = float(light_voxelSize) / CRAZY_NUM;   // get step size form grid
    cout << "	 | Light voxel size: " << light_voxelSize << endl;
    cout << "	 | Light step size: " << light_step_size << endl;
    LightVolume lightVolume(myLights, *finalDensityPtr, *finalMatColorPtr, K, light_step_size, light_voxelSize, finalBBox);

    cout << "Set image..." << endl;
    Image myImg;
    myImg.reset(WEIGHT, HEIGHT);
    cout << "Set camera..." << endl;
    Camera myCamera;
    Vector eye(22.0, 0.0, 0.0);
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
    cout << "	 | Pyro bounding box: " << pyroNewBBox.min() << " " << pyroNewBBox.max() << endl;
    // gridded
    static FloatGridVolume pyroSphereVolume(pyroSphereGrid);

    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantFloat rho(7.0);
    static ConstantColor pyroMatColor(matColor);
    static ConstantColor pyroEmColor(emColor);
    static DensityVolume pyroDensity(rho, pyroSphereVolume);

    K = 3;
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
    Color keyColor(2.0, 2.0, 2.0, 1.0);
    Color rimColor(0.2, 0.2, 0.2, 1.0);
    // set lights
    LightSource keyLight(keyPos, keyColor);
    LightSource rimLight(rimPos, rimColor);
    myLights.push_back(keyLight);
    myLights.push_back(rimLight);
    // get light step size
    float bboxSize_x = (finalBBox.max().x() - finalBBox.min().x());
    float bboxSize_y = (finalBBox.max().y() - finalBBox.min().y());
    float bboxSize_z = (finalBBox.max().z() - finalBBox.min().z());
    float bboxSize;
    bboxSize = getMin(bboxSize_x, bboxSize_y);
    bboxSize = getMin(bboxSize, bboxSize_z);
    cout << "	 | BBox size: " << bboxSize << endl;
    float light_voxelSize = float(bboxSize) / LIGHT_GRID_NUM_1;
    float light_step_size = float(light_voxelSize) / CRAZY_NUM;   // get step size form grid
    cout << "	 | Light voxel size: " << light_voxelSize << endl;
    cout << "	 | Light step size: " << light_step_size << endl;
    // get final light volume
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
    static Noise_t FSPN2Parms;
    FSPN2Parms.octaves = wispparms.octaves;
    FSPN2Parms.frequency = wispparms.freq;
    FSPN2Parms.fjump = wispparms.fjump;
    FSPN2Parms.roughness = 1.2;

    // FSPN1Parms.frequency = 2.12032;

    FSPN1.setParameters(FSPN1Parms);
    FSPN2.setParameters(FSPN2Parms);

    static WispParms wispParameters;
    wispParameters.clump = wispparms.clump;
    wispParameters.FSPN2 = &FSPN2;

    float voxelSize = 4.5 / 500;
    step_size = float(voxelSize) / CRAZY_NUM;   // get step size form grid

    cout << "Create wisp grid..." << endl;
    static SingleGuideWisp wisp(FSPN1, wispParameters, voxelSize, 5000000);
    static FloatGrid::Ptr wispGrid = wisp.getWispGrid();
    static BBox wispBBox = wisp.getBBox();
    cout << "	 | Wisp bounding box: " << wispBBox.min() << " " << wispBBox.max() << endl;
    static FloatGridVolume wispVolume(wispGrid);

    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantFloat rho(0.6);
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
    cout << "Noise 2 octaves: " << myWispParms.octaves << endl;
    cout << "Noise 2 freq: " << myWispParms.freq << endl;
    cout << "Noise 2 fjump: " << myWispParms.fjump << endl;
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
    Color keyColor(1.0, 3.9, 5.0, 1.0);
    Color rimColor(0.21, 0.5, 0.4, 1.0);
    // set lights
    LightSource keyLight(keyPos, keyColor);
    LightSource rimLight(rimPos, rimColor);
    myLights.push_back(keyLight);
    myLights.push_back(rimLight);
    // get light step size
    float bboxSize_x = (finalBBox.max().x() - finalBBox.min().x());
    float bboxSize_y = (finalBBox.max().y() - finalBBox.min().y());
    float bboxSize_z = (finalBBox.max().z() - finalBBox.min().z());
    float bboxSize;
    bboxSize = getMin(bboxSize_x, bboxSize_y);
    bboxSize = getMin(bboxSize, bboxSize_z);
    cout << "	 | BBox size: " << bboxSize << endl;
    float light_voxelSize = float(bboxSize) / LIGHT_GRID_NUM_2;
    float light_step_size = float(light_voxelSize) / CRAZY_NUM;   // get step size form grid
    cout << "	 | Light voxel size: " << light_voxelSize << endl;
    cout << "	 | Light step size: " << light_step_size << endl;

    LightVolume lightVolume(myLights, *finalDensityPtr, *finalMatColorPtr, K, light_step_size, light_voxelSize, finalBBox);

    cout << "Set image..." << endl;
    Image myImg;
    myImg.reset(WEIGHT, HEIGHT);
    cout << "Set camera..." << endl;
    Camera myCamera;
    Vector eye(17.0, 0.0, 0.0);
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
