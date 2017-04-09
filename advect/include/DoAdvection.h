# ifndef __DOADVECTION_H__
# define __DOADVECTION_H__

# include <iostream>
# include <vector>
# include "omp.h"
# include <cmath>
# include <openvdb/openvdb.h>

# include "Types.h"
# include "Noise.h"
# include "PerlinNoise.h"
# include "Pyroclastic.h"
# include "Camera.h"
# include "Color.h"
# include "Grid.h"
# include "Renderer.h"
# include "OIIOFiles.h"
# include "Operations.h"
# include "Vector.h"
# include "PolyModel.h"
# include "Levelsets.h"
# include "VDBTool.h"
# include "Advect.h"
# include "VectorField.h"

using namespace std;
using namespace lux;

// levelsets
# define VOXEL_SIZE 0.003
# define HALF_NW 80

// stamping
# define GRID_VOXEL_SIZE 0.008

// volume property
# define KAPPA 1.0
# define RHO 16.0

// renderer
# define LIGHT_GRID_NUM 50

// camera
# define WEIGHT 960
# define HEIGHT 540
# define NEAR 0.1
# define FAR 500

// constant
# define CRAZY_NUM 2.0316578

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

string levelsetsPath = "../tmp/bunny_levelsets.vdb";
string levelsetsGridName = "bunny_levelsets";

string pyroPath = "../tmp/bunny_pyro.vdb";
string pyroGridName = "bunny_pyro";

string advectPathTemplate = "../tmp/bunny_advect.%04d.vdb";
string advectGridNameTemplate = "bunny_advect.%04d";

float step_size;


// ---------------------------------------- bunny pyroclast --------------------------------------------

FloatGrid::Ptr loadBunnyPyro(int id, BBox& bbox)
{
    /// ----------------------------------- Load Levelsets --------------------------------------------

    // load bunny levelsets from .vdb file
    FloatGrid::Ptr bunnyGrid = readVDBFloatGrid(levelsetsPath, levelsetsGridName);
    BBox bunnyLevelsetsBBox = getGridBBox(bunnyGrid);
    cout << "\t | Bunny Levelsets BBox: " << bunnyLevelsetsBBox.min() << " " << bunnyLevelsetsBBox.max() << endl;

    /// ----------------------------------- Create Pyroclasts -----------------------------------------

    float maxNoiseAmp = bunnyGrid->background();    // background = halfwidth * voxelSize
    float noiseAmp = maxNoiseAmp * id / float(59);

    cout << "Create perlin noise..." << endl;
    Noise_t parms;
    parms.gamma = 1.5;
    parms.frequency = 9.57434;
    parms.fjump = 2.6;
    parms.octaves = 0.5;
    parms.amplitude = noiseAmp;
    static FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    static PyroVDBLevelsets bunnyPyroVolume(bunnyGrid, perlin);

    cout << "Stamping pyroclastic bunny..." << endl;
    static FloatVolumeToGrid bunnyPyroV2Grid(bunnyPyroVolume, GRID_VOXEL_SIZE, bunnyLevelsetsBBox);
    static FloatGrid::Ptr bunnyPyroGrid = bunnyPyroV2Grid.getVolumeGrid();
    BBox bunnyPyroBBox = bunnyPyroV2Grid.getBBox();
    cout << "\t | bunnyPyroV2Grid bbox: " << bunnyPyroBBox.min() << bunnyPyroBBox.max() << endl;

    bbox = bunnyPyroBBox;

    return bunnyPyroGrid;
}


// ---------------------------------------- bunny advection --------------------------------------------

void createCMGrid()
{
    cout << "Create perlin noise..." << endl;
    Noise_t parms;
//    parms.gamma = 1.5;
//    parms.frequency = 9.57434;
//    parms.fjump = 2.6;
//    parms.octaves = 0.5;
    FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    // generate velocity field from vector noise
    cout << "Create velocity field..." << endl;
    Vector delta_x(0.1, 0.1, 0.1);
    Vector delta_y(0.1, 0.0, -0.1);
    Vector delta_z(0.0, 0.1, 0.0);
    VNoise1 velocityField(perlin, delta_x, delta_y, delta_z);

    // initialization
    Identity identity;
    VolumeVectorPtr X = &identity;
    BBox advectBBox(Vec3s(-2, -2, -2), Vec3s(2, 2, 2));
    // semi-lagrangian mapping
    for (int id = 1; id <= 3; ++id)
    {
        // get grid name and grid path
        char advectGridName[1024];
        sprintf(advectGridName, advectGridNameTemplate.c_str(), id);
        char advectPath[1024];
        sprintf(advectPath, advectPathTemplate.c_str(), id);

        float delta_t = id;
        VectorAdvectPtr vecAdvectPtr = new VectorAdvect(X, &velocityField, delta_t);

        // generate characteristic map grid
        cout << "Stamping characteristic map " << advectGridName << "..." << endl;
        VectorVolumeToGrid vecAdvectV2Grid(*vecAdvectPtr, GRID_VOXEL_SIZE, advectBBox);
        Vec3fGrid::Ptr vecAdvectGrid = vecAdvectV2Grid.getVolumeGrid();

        // write levelsets grid into file
        openvdb::GridPtrVec grids;
        // push into container
        vecAdvectGrid->setName(advectGridName);
        vecAdvectGrid->setGridClass(openvdb::GRID_UNKNOWN);
        grids.push_back(vecAdvectGrid);
        cout << "Writing grid " << advectGridName << " into file: " << advectPath << "..." << endl;
        writeVDBGrid(grids, advectPath);

        // gridded
        VolumeVectorPtr vecAdvectVolume = new VectorGridVolume(vecAdvectGrid);
        X = vecAdvectVolume;
    }
}


FloatGrid::Ptr loadBunnyAdvect(int frame_id, BBox& bbox)
{
    /// ----------------------------------- Load Pyrobunny --------------------------------------------

    int id = frame_id - 59;

    // load pyroclastic bunny from .vdb file
    FloatGrid::Ptr pyrobunnyGrid = readVDBFloatGrid(pyroPath, pyroGridName);
    BBox pyrobunnyGridBBox = getGridBBox(pyrobunnyGrid);
    cout << "\t | Bunny Levelsets BBox: " << pyrobunnyGridBBox.min() << " " << pyrobunnyGridBBox.max() << endl;

    // get grid name and grid path
    char advectGridName[1024];
    char advectPath[1024];
    sprintf(advectGridName, advectGridNameTemplate.c_str(), id);
    sprintf(advectPath, advectPathTemplate.c_str(), id);

    // load characteristic map from .vdb file
    Vec3fGrid::Ptr advectGrid = readVDBVectorGrid(advectPath, advectGridName);

    // gridded for density volume and characteristic map volume
    FloatGridVolume densityVolume(pyrobunnyGrid);
    VectorGridVolume advectVolume(advectGrid);
    VolumeFloatPtr densityVolumePtr = &densityVolume;
    VolumeVectorPtr advectVolumePtr = &advectVolume;
    Warp warp(densityVolumePtr, advectVolumePtr);

    // test pyrobunny
    bbox = pyrobunnyGridBBox;

    return pyrobunnyGrid;
}

// ----------------------------------- Volume Rendering ------------------------------------------------

void assignVolumeProperty(FloatGrid::Ptr grid,
                          VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
                          VolumeColorPtr& finalMatColorPtr, float& K)
{
    VolumeFloatPtr volume = new FloatGridVolume(grid);
    // create bunny color volume and density volume
    static Color matColor(1.0, 1.0, 1.0, 1.0);
    static Color emColor(0.0, 0.0, 0.0, 0.0);
    static ConstantColor bunnyMatColor(matColor);
    static ConstantColor bunnyEmColor(emColor);
    static ConstantFloat rho(100.0);
    static DensityVolume bunnyDensity(rho, *volume);
    // set K
    K = 0.8;
    finalDensityPtr = &bunnyDensity;
    finalEmColorPtr = &bunnyEmColor;
    finalMatColorPtr = &bunnyMatColor;
}


void createBunnyCumulo(int frame_id, string output_path)
{
    /// ----------------------------------- Initialization --------------------------------------------

    cout << "frame_id: " << frame_id << endl;
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw5_cumulo.%04d.exr", output_path.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;
    cout << "--------------------------------------------" << endl;

    /// ----------------------------------- Cumulo Setup ----------------------------------------------

    VolumeFloatPtr finalDensityPtr;
    VolumeColorPtr finalEmColorPtr;
    VolumeColorPtr finalMatColorPtr;
    BBox finalBBox;
    float K;

    FloatGrid::Ptr bunnyCumuloGrid;
    // first 60 frames: do pyroclasts
    if (frame_id <= 59) {bunnyCumuloGrid = loadBunnyPyro(frame_id, finalBBox);}
    // remaining 60 frames: do advection
    else    {bunnyCumuloGrid = loadBunnyAdvect(frame_id, finalBBox);}
    // assign color, opacity, density to volume
    assignVolumeProperty(bunnyCumuloGrid, finalDensityPtr, finalEmColorPtr, finalMatColorPtr, K);

    /// ---------------------------------- Lighting & Rendering ---------------------------------------

    step_size = GRID_VOXEL_SIZE / CRAZY_NUM;

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
