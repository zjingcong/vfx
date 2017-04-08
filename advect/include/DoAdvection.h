# ifndef __DOADVECTION_H__
# define __DOADVECTION_H__

# include <iostream>
# include <vector>
# include "omp.h"
# include <cmath>

# include "Types.h"
# include "Noise.h"
# include "PerlinNoise.h"
# include "Pyroclastic.h"
# include "Camera.h"
# include "Color.h"
# include "Grid.h"
# include "Renderer.h"
# include "OIIOFiles.h"
# include "ScalarVolumeXform.h"
# include "Operations.h"
# include "Vector.h"
# include "PolyModel.h"
# include "Levelsets.h"

using namespace std;
using namespace lux;

# define WEIGHT 960
# define HEIGHT 540
# define LIGHT_GRID_NUM 50
# define VOXEL_SIZE 0.003
# define GRID_VOXEL_SIZE 0.008
# define HALF_NW 80

# define CRAZY_NUM 2.0316578
# define NEAR 0.1
# define FAR 500
# define KAPPA 1.0
# define RHO 16.0

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

float step_size;


void loadBunny(VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
               VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float& K)
{
    string bunnyPath = "../models/cleanbunny.obj";
    // load bunny model
    static PolyModel polyBunny;
    polyBunny.loadObj(bunnyPath);
    cout << "--------------------------------------------" << endl;

    // generate bunny levelsets
    cout << "Create levelsets..." << endl;
    cout << "\t | Half Narrow Band: " << VOXEL_SIZE * HALF_NW << endl;
    static PolyLevelsets bunnyLevelsets(polyBunny, VOXEL_SIZE, HALF_NW);
    static VDBLevelsetsPtr bunnyGrid = bunnyLevelsets.getVDBLevelsets();
    BBox bunnyLevelsetsBBox = bunnyLevelsets.getBBox();
    cout << "\t | Bunny Levelsets BBox: " << bunnyLevelsetsBBox.min() << " " << bunnyLevelsetsBBox.max() << endl;

    cout << "Create perlin noise..." << endl;
    Noise_t parms;
    parms.gamma = 1.5;
    parms.frequency = 9.57434;
    parms.fjump = 2.6;
    parms.octaves = 0.5;
    parms.amplitude = 0.3;
    static FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    static PyroVDBLevelsets bunnyPyroVolume(bunnyGrid, perlin);

    cout << "Stamping pyroclastic bunny..." << endl;
    static FloatVolumeToGrid bunnyPyroV2Grid(bunnyPyroVolume, GRID_VOXEL_SIZE, bunnyLevelsetsBBox);
    static FloatGrid::Ptr bunnyPyroGrid = bunnyPyroV2Grid.getVolumeGrid();
    BBox bunnyPyroBBox = bunnyPyroV2Grid.getBBox();
    cout << "\t | bunnyPyroV2Grid bbox: " << bunnyPyroBBox.min() << bunnyPyroBBox.max() << endl;
    static FloatGridVolume bunnyPyro(bunnyPyroGrid);

    // create bunny color volume and density volume
    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantColor bunnyMatColor(matColor);
    static ConstantColor bunnyEmColor(emColor);
    static ConstantFloat rho(100.0);
    static DensityVolume bunnyDensity(rho, bunnyPyro);

    // set K
    K = 0.8;
    finalDensityPtr = &bunnyDensity;
    finalEmColorPtr = &bunnyEmColor;
    finalMatColorPtr = &bunnyMatColor;
    finalBBox = bunnyLevelsetsBBox;
}


void createBunnyCumulo(int frame_id, string output_path)
{
    /// ----------------------------------- Initialization --------------------------------------------

    cout << "frame_id: " << frame_id << endl;
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw5_cumulo.%04d.exr", output_path.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;

    /// ----------------------------------- Cumulo Setup ----------------------------------------------

    VolumeFloatPtr finalDensityPtr;
    VolumeColorPtr finalEmColorPtr;
    VolumeColorPtr finalMatColorPtr;
    BBox finalBBox;
    float K;
    loadBunny(finalDensityPtr, finalEmColorPtr, finalMatColorPtr, finalBBox, K);

    /// ---------------------------------- Lighting & Rendering ---------------------------------------

    step_size = GRID_VOXEL_SIZE / CRAZY_NUM;

    cout << "--------------------------------------------" << endl;
    // lighting
    cout << "Set lights..." << endl;
    Lights myLights;
    // light position
    Vector keyPos(0.0, 3.0, 0.0);
    Vector rimPos(0.0, -3.0, 0.0);
    // light color
    Color keyColor(0.0, 0.5, 0.25, 1.0);
    Color rimColor(0.2, 0.2, 0.0, 1.0);
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
    float light_voxelSize = float(bboxSize) / LIGHT_GRID_NUM;
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
    Vector eye(0.0, 0.0, 4.0);
    Vector view(0.0, 0.0, -1.0);
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
