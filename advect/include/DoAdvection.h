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

//# define WEIGHT 1920
//# define HEIGHT 1080
//# define GRID_NUM 130
//# define COLOR_GRID_NUM 70
//# define LIGHT_GRID_NUM 80

// test parms
# define WEIGHT 960
# define HEIGHT 540
# define GRID_NUM 50
# define LIGHT_GRID_NUM 100
# define VOXEL_SIZE 0.004
# define HALF_NW 3

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
    static PolyLevelsets bunnyLevelsets(true, polyBunny, VOXEL_SIZE);
    static FloatGrid::Ptr bunnyGrid = bunnyLevelsets.getLevelsets();
    BBox bunnyBBox = bunnyLevelsets.getBBox();
    cout << "\t | Bunny BBox: " << bunnyBBox.min() << " " << bunnyBBox.max() << endl;

    // generate bunny volume
    cout << "Stamping model..." << endl;
    static FloatGridVolume bunnyVolume(bunnyGrid);

    // create bunny color volume and density volume
    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantColor bunnyMatColor(matColor);
    static ConstantColor bunnyEmColor(emColor);
    static ConstantFloat rho(500.0);
    static DensityVolume bunnyDensity(rho, bunnyVolume);

    // set K
    K = 0.8;
    finalDensityPtr = &bunnyDensity;
    finalEmColorPtr = &bunnyEmColor;
    finalMatColorPtr = &bunnyMatColor;
    finalBBox = bunnyBBox;
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

    step_size = 0.01;

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
    Vector eye(0.0, 0.0, 3.5);
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
