# ifndef __DOADVECTION_H__
# define __DOADVECTION_H__

# include <iostream>
# include <vector>
# include "omp.h"
# include <cmath>
# include <openvdb/openvdb.h>

# include "Types.h"
# include "ConfigParser.h"
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
# define CM_GRID_VOXEL_SIZE 0.01

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

// vdb files
string levelsetsPath = "./DPA/jedi/zjingcong/grids/advect_grids/bunny_levelsets.vdb";
string levelsetsGridName = "bunny_levelsets";

string pyroPath = "/DPA/jedi/zjingcong/grids/advect_grids/bunny_pyro.vdb";
string pyroGridName = "bunny_pyro";

string advectPathTemplate = "/DPA/jedi/zjingcong/grids/advect_grids/bunny_advect.%04d.vdb";
string advectGridNameTemplate = "bunny_advect.%04d";

// config file
string advectCfgName = "advect.cfg";
string pyroCfgName = "pyro.cfg";
string advectConfigPath;
string pyroConfigPath;


// -------------------------------------------- setup --------------------------------------------------

void setCfgPath(string cfgPath)
{
    advectConfigPath = cfgPath + "/" + advectCfgName;
    pyroConfigPath = cfgPath + "/" + pyroCfgName;
    cout << "advectConfigPath: " << advectConfigPath << endl;
    cout << "pyroConfigPath: " << pyroConfigPath << endl;
}


// ---------------------------------------- bunny pyroclast --------------------------------------------

FloatGrid::Ptr loadBunnyPyro(int id, BBox& bbox)
{
    /// ----------------------------------- Load Levelsets --------------------------------------------

    // load bunny levelsets from .vdb file
    FloatGrid::Ptr bunnyGrid = readVDBGrid<FloatTree>(levelsetsPath, levelsetsGridName);
    BBox bunnyLevelsetsBBox = getGridBBox<FloatTree>(bunnyGrid);
    cout << "\t | Bunny Levelsets BBox: " << bunnyLevelsetsBBox.min() << " " << bunnyLevelsetsBBox.max() << endl;

    /// ----------------------------------- Create Pyroclasts -----------------------------------------

    float maxNoiseAmp = bunnyGrid->background();    // background = halfwidth * voxelSize
    float noiseAmp = maxNoiseAmp * id / float(59);

    // get pyro parms config
    cout << "Get pyroclasts parms..." << endl;
    cfg::FloatValueMap cfgPyroParms;
    cfgPyroParms = cfg::floatValueParser(pyroConfigPath);

    cout << "Create perlin noise..." << endl;
    Noise_t parms;
    parms.gamma = cfgPyroParms.at("gamma");
    parms.frequency = cfgPyroParms.at("frequency");
    parms.fjump = cfgPyroParms.at("fjump");
    parms.octaves = cfgPyroParms.at("octaves");
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
    // get advect parms config
    cout << "Get advection parms..." << endl;
    cfg::FloatValueMap cfgAdvectParms;
    cfgAdvectParms = cfg::floatValueParser(advectConfigPath);

    cout << "Create perlin noise..." << endl;
    // set noise parms
    Noise_t parms;
    parms.gamma = cfgAdvectParms.at("gamma");
    parms.frequency = cfgAdvectParms.at("frequency");
    parms.fjump = cfgAdvectParms.at("fjump");
    parms.octaves = cfgAdvectParms.at("octaves");
    // create perlin noise
    FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    float timestep = cfgAdvectParms.at("timestep");
    int stepnum = int(cfgAdvectParms.at("stepnum"));
    float offset = cfgAdvectParms.at("offset");

    // generate velocity field from vector noise
    cout << "Create velocity field..." << endl;
    Vector delta_x = offset * Vector(1.0, 1.0, 1.0);
    Vector delta_y = offset * Vector(1.0, 0.0, -1.0);
    Vector delta_z = offset * Vector(0.0, 1.0, 0.0);
    VNoise1 velocityField(perlin, delta_x, delta_y, delta_z);

    // initialization
    Identity identity;
    VectorGridVolumePtr X;
    BBox advectBBox(Vec3s(-1.5, -1.5, -1.5), Vec3s(1.5, 1.5, 1.5));
    // semi-lagrangian mapping
    for (int id = 1; id <= stepnum; ++id)
    {
        // get grid name and grid path
        char advectGridName[1024];
        sprintf(advectGridName, advectGridNameTemplate.c_str(), id);
        char advectPath[1024];
        sprintf(advectPath, advectPathTemplate.c_str(), id);

        float delta_t = id * timestep;
        VolumeVectorPtr vecAdvectPtr;
        if (id == 1)
        { vecAdvectPtr = new VectorAdvect(&identity, &velocityField, delta_t); }
        else
        { vecAdvectPtr = new VectorAdvect(X, &velocityField, delta_t); }

        // generate characteristic map grid
        cout << "Stamping characteristic map " << advectGridName << "..." << endl;
        VectorVolumeToGrid vecAdvectV2Grid(*vecAdvectPtr, CM_GRID_VOXEL_SIZE, advectBBox);
        Vec3fGrid::Ptr vecAdvectGrid = vecAdvectV2Grid.getVolumeGrid();

        // release memory
        if (id > 1)
        {
            Vec3fGrid::Ptr preVecAdvectGrid = X->getGrid();
            preVecAdvectGrid->clear();
            delete X;
        }

        // write levelsets grid into file
        openvdb::GridPtrVec grids;
        // push into container
        vecAdvectGrid->setName(advectGridName);
        vecAdvectGrid->setGridClass(openvdb::GRID_UNKNOWN);
        grids.push_back(vecAdvectGrid);
        cout << "Writing grid " << advectGridName << " into file: " << advectPath << "..." << endl;
        writeVDBGrid(grids, advectPath);

        // gridded
        VectorGridVolumePtr vecAdvectVolume = new VectorGridVolume(vecAdvectGrid);
        X = vecAdvectVolume;
    }
}


// create single characteristic map grid from .vdb file
void createSingleCMGrid(int id)
{
    // get advect parms config
    cout << "Get advection parms..." << endl;
    cfg::FloatValueMap cfgAdvectParms;
    cfgAdvectParms = cfg::floatValueParser(advectConfigPath);

    cout << "Create perlin noise..." << endl;
    // set noise parms
    Noise_t parms;
    parms.gamma = cfgAdvectParms.at("gamma");
    parms.frequency = cfgAdvectParms.at("frequency");
    parms.fjump = cfgAdvectParms.at("fjump");
    parms.octaves = cfgAdvectParms.at("octaves");
    // create perlin noise
    FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    float timestep = cfgAdvectParms.at("timestep");
    int stepnum = int(cfgAdvectParms.at("stepnum"));
    float offset = cfgAdvectParms.at("offset");

    // generate velocity field from vector noise
    cout << "Create velocity field..." << endl;
    Vector delta_x = offset * Vector(1.0, 1.0, 1.0);
    Vector delta_y = offset * Vector(1.0, 0.0, -1.0);
    Vector delta_z = offset * Vector(0.0, 1.0, 0.0);
    VNoise1 velocityField(perlin, delta_x, delta_y, delta_z);

    // get grid name and grid path
    char advectGridName[1024];
    sprintf(advectGridName, advectGridNameTemplate.c_str(), id);
    char advectPath[1024];
    sprintf(advectPath, advectPathTemplate.c_str(), id);

    // initialization
    Identity identity;
    BBox advectBBox(Vec3s(-1.5, -1.5, -1.5), Vec3s(1.5, 1.5, 1.5));
    // load previous grid
    VectorGridVolumePtr X;
    if (id > 1)
    {
        char preGridName[1024];
        sprintf(preGridName, advectGridNameTemplate.c_str(), (id - 1));
        char preGridPath[1024];
        sprintf(preGridPath, advectPathTemplate.c_str(), (id - 1));
        Vec3fGrid::Ptr preGrid = readVDBGrid<Vec3fTree>(preGridPath, preGridName);
        X = new VectorGridVolume(preGrid);
    }

    // semi-lagrangian mapping
    float delta_t = id * timestep;
    VolumeVectorPtr vecAdvectPtr;
    if (id == 1)
    { vecAdvectPtr = new VectorAdvect(&identity, &velocityField, delta_t); }
    else
    { vecAdvectPtr = new VectorAdvect(X, &velocityField, delta_t); }

    // generate characteristic map grid
    cout << "Stamping characteristic map " << advectGridName << "..." << endl;
    VectorVolumeToGrid vecAdvectV2Grid(*vecAdvectPtr, CM_GRID_VOXEL_SIZE, advectBBox);
    Vec3fGrid::Ptr vecAdvectGrid = vecAdvectV2Grid.getVolumeGrid();

    // write levelsets grid into file
    openvdb::GridPtrVec grids;
    // push into container
    vecAdvectGrid->setName(advectGridName);
    vecAdvectGrid->setGridClass(openvdb::GRID_UNKNOWN);
    grids.push_back(vecAdvectGrid);
    cout << "Writing grid " << advectGridName << " into file: " << advectPath << "..." << endl;
    writeVDBGrid(grids, advectPath);

    // release memory
    if (id > 1)
    {
        Vec3fGrid::Ptr preVecAdvectGrid = X->getGrid();
        preVecAdvectGrid->clear();
        delete X;
    }
    vecAdvectGrid->clear();
    delete vecAdvectPtr;
}


FloatGrid::Ptr loadBunnyAdvect(int frame_id, BBox& bbox)
{
    /// ----------------------------------- Load Pyrobunny --------------------------------------------

    int id = frame_id - 59;

    // load pyroclastic bunny from .vdb file
    FloatGrid::Ptr pyrobunnyGrid = readVDBGrid<FloatTree>(pyroPath, pyroGridName);
    BBox pyrobunnyGridBBox = getGridBBox<FloatTree>(pyrobunnyGrid);
    cout << "\t | Bunny Pyro BBox: " << pyrobunnyGridBBox.min() << " " << pyrobunnyGridBBox.max() << endl;

    // get grid name and grid path
    char advectGridName[1024];
    char advectPath[1024];
    sprintf(advectGridName, advectGridNameTemplate.c_str(), id);
    sprintf(advectPath, advectPathTemplate.c_str(), id);

    // load characteristic map from .vdb file
    Vec3fGrid::Ptr advectGrid = readVDBGrid<Vec3fTree>(advectPath, advectGridName);
    BBox CMBBox = getGridBBox<Vec3fTree>(advectGrid);

    // gridded for density volume and characteristic map volume
    static FloatGridVolume densityVolume(pyrobunnyGrid);
    static VectorGridVolume advectVolume(advectGrid);
    VolumeFloatPtr densityVolumePtr = &densityVolume;
    VolumeVectorPtr advectVolumePtr = &advectVolume;
    static Warp warpedBunny(densityVolumePtr, advectVolumePtr);

    // stamping warped density into grid
    cout << "Stamping advection bunny..." << endl;
    static FloatVolumeToGrid bunnyAdvectV2Grid(warpedBunny, GRID_VOXEL_SIZE, CMBBox);
    static FloatGrid::Ptr bunnyAdvectGrid = bunnyAdvectV2Grid.getVolumeGrid();
    BBox bunnyAdvectBBox = bunnyAdvectV2Grid.getBBox();
    cout << "\t | bunnyAdvectV2Grid bbox: " << bunnyAdvectBBox.min() << bunnyAdvectBBox.max() << endl;

    bbox = bunnyAdvectBBox;

    return bunnyAdvectGrid;
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

    float step_size = GRID_VOXEL_SIZE / CRAZY_NUM;

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

