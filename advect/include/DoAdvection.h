# ifndef __DOPYROS_H__
# define __DOPYROS_H__

# include <iostream>
# include <vector>
# include "omp.h"
# include <cmath>

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
# define COLOR_GRID_NUM 20
# define LIGHT_GRID_NUM 40

# define NEAR 0.1
# define FAR 500
# define CRAZY_NUM 2.0316578
# define KAPPA 1.0
# define RHO 16.0

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

//float step_size;


//void loadBunny(VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
//               VolumeColorPtr& finalMatColorPtr, BBox& finalBBox, float& K)
//{
//    string bunnyPath = "../models/cleanbunny.obj";
//    // load bunny model
//    static PolyModel polyBunny;
//    polyBunny.loadObj(bunnyPath);
//    cout << "--------------------------------------------" << endl;
//    // generate bunny levelsets
//    cout << "Create levelsets..." << endl;
//    static PolyLevelsets bunnyLevelsets(true, polyBunny, 5, 0.004);
//    static FloatGrid::Ptr bunnyGrid = bunnyLevelsets.getLevelsets();
//
//    // generate bunny volume
//    cout << "Stamping model..." << endl;
//    static VDBLevelsetsVolume bunnyVolume(bunnyGrid);
//    // generate bunny BBox
//    static Vec3s bunnyLLC(polyBunny.x_min, polyBunny.y_min, polyBunny.z_min);
//    static Vec3s bunnyURC(polyBunny.x_max, polyBunny.y_max, polyBunny.z_max);
//    static BBox bunnyBBox(bunnyLLC, bunnyURC);
//
//    // create bunny color volume and density volume
//    static Color matColor(1.0, 1.0, 1.0, 1.0);
//    static Color emColor(0.0, 0.0, 0.0, 0.0);
//    static ConstantColor bunnyMatColor(matColor);
//    static ConstantColor bunnyEmColor(emColor);
//    static ConstantFloat rho(600.0);
//    static DensityVolume bunnyDensity(rho, bunnyVolume);
//
//    // set K
//    K = 0.8;
//    finalDensityPtr = &bunnyDensity;
//    finalEmColorPtr = &bunnyEmColor;
//    finalMatColorPtr = &bunnyMatColor;
//    finalBBox = bunnyBBox;
//}
//
//
//FloatGrid::Ptr loadBunnyGrid(BBox& bbox)
//{
//    string bunnyPath = "../models/cleanbunny.obj";
//    // load bunny model
//    static PolyModel polyBunny;
//    polyBunny.loadObj(bunnyPath);
//    cout << "--------------------------------------------" << endl;
//    // generate bunny levelsets
//    cout << "Create levelsets..." << endl;
//    static PolyLevelsets bunnyLevelsets(true, polyBunny, 5, 0.004);
//    static FloatGrid::Ptr bunnyGrid = bunnyLevelsets.getLevelsets();
//
//    // generate bunny BBox
//    static Vec3s bunnyLLC(polyBunny.x_min, polyBunny.y_min, polyBunny.z_min);
//    static Vec3s bunnyURC(polyBunny.x_max, polyBunny.y_max, polyBunny.z_max);
//    static BBox bunnyBBox(bunnyLLC, bunnyURC);
//
//    bbox = bunnyBBox;
//
//    return bunnyGrid;
//}


//void createBunnyCumulo(int frame_id, string output_path)
//{
//    /// ----------------------------------- Initialization ---------------------------------------------
//    cout << "frame_id: " << frame_id << endl;
//    char file_name[1024];
//    sprintf(file_name, "%s/jingcoz_hw4.%04d.exr", output_path.c_str(), frame_id);
//    cout << "Output path: " << file_name << endl;
//
//    // get parms for pyrolist
//    vector<Vector> transVecs = createTransVecs();
//    vector<Noise_t*> noiseParmsVecs = createNoiseParmsVecs(frame_id);
//    createWave(frame_id, noiseParmsVecs);
//
//    /// ----------------------------------- Pyroclasts Setup -------------------------------------------
//
//    ConstantFloat init(0.0);
//    BBox pyroBBox;
//    vector<VolumeFloatPtr> pyroVolumePtrs;
//    vector<VolumeColorPtr> pyroEmVolumePtrs;
//
//    // generate 36 pyrospheres
//    for (int i = 0; i < 36; i++)
//    {
//        Vector transVec = transVecs.at(i);
//        Vec3s transVec3s(transVec.X(), transVec.Y(), transVec.Z());
//
//        FractalSum<PerlinNoiseGustavson> *perlin = new FractalSum<PerlinNoiseGustavson>;
//        perlin->setParameters(*noiseParmsVecs.at(i));
//
//        Pyrosphere* pyrosphereOrigin = new Pyrosphere(*perlin);
//        BBox pyrosphereBBoxOrigin = pyrosphereOrigin->getBBox();
//        // translate
//        VolumeFloatPtr pyrosphereTrans = new ScalarTranslate(*pyrosphereOrigin, transVec);
//        pyrosphereBBoxOrigin.translate(transVec3s);
//        pyroVolumePtrs.push_back(pyrosphereTrans);
//        // union bbox
//        if (i == 0)  {pyroBBox = pyrosphereBBoxOrigin;}
//        else {pyroBBox.expand(pyrosphereBBoxOrigin);}
//        // set color for each pyro
//        VolumeColorPtr pyrosphereEm = new ConstantColor(colorVecs.at(i));
//        VolumeColorPtr pyrosphereEmVolume = new ColorVolume(*pyrosphereEm, *pyrosphereTrans);
//        pyroEmVolumePtrs.push_back(pyrosphereEmVolume);
//    }
//
//    // union pyroclasts
//    ImplicitUnionList pyro(pyroVolumePtrs);
//    VolumeFloatPtr pyroVolumePtr = &pyro;
//    // add colors
//    ColorAddList pyroEmColorAdd(pyroEmVolumePtrs);
//
//    // get grid voxelSize
//    float voxelSize = float(pyroBBox.max().y() - pyroBBox.min().y()) / GRID_NUM;
//    step_size = float(voxelSize) / CRAZY_NUM;   // get step size form grid
//    // stamp
//    cout << "Stamping..." << endl;
//    cout << "	 | Grid voxel size: " << voxelSize << endl;
//    cout << "Stamping pyro density..." << endl;
//    FloatVolumeToGrid pyrosVolume2Grid(*pyroVolumePtr, voxelSize, pyroBBox);
//    FloatGrid::Ptr pyrosGrid = pyrosVolume2Grid.getVolumeGrid();
//    BBox pyroNewBBox = pyrosVolume2Grid.getBBox();
//    cout << "Stamping pyro emcolor..." << endl;
//    float colorVoxelSize = float(pyroNewBBox.max().y() - pyroNewBBox.min().y()) / COLOR_GRID_NUM;
//    cout << "	 | Grid color voxel size: " << colorVoxelSize << endl;
//    ColorVolumeToGrid pyroEm2Grid(pyroEmColorAdd, colorVoxelSize, pyroNewBBox);
//    Vec4fGrid::Ptr pyroEmGrid = pyroEm2Grid.getVolumeGrid();
//    // release memory
//    for (VolumeFloatPtr v: pyroVolumePtrs)  { delete v; }
//    // gridded
//    FloatGridVolume pyrosVolume(pyrosGrid);
//    ColorGridVolume pyroEmColor(pyroEmGrid);
//
//    // set matcolor, opacity and density
//    Color matColor(1.0, 1.0, 1.0, 1.0);
//    ConstantColor pyroMatColor(matColor);
//    ConstantFloat rho(RHO);
//    DensityVolume pyroDensity(rho, pyrosVolume);
//    float K = KAPPA;
//    BBox finalBBox = pyroNewBBox;
//    cout << "	 | Pyro bounding box: " << finalBBox.min() << " " << finalBBox.max() << endl;
//
//    /// ---------------------------------- Lighting & Rendering ------------------------------------
//
//    // lighting
//    cout << "Set lights..." << endl;
//    Lights myLights;
//    // light position
//    Vector keyPos(-5.0, 6.0, -5.0);
//    Vector rimPos(10.0, -10.0, 10.0);
//    // light color
//    Color keyColor(0.2, 0.2, 0.2, 1.0);
//    Color rimColor(0.04, 0.04, 0.04, 1.0);
//    // set lights
//    LightSource keyLight(keyPos, keyColor);
//    LightSource rimLight(rimPos, rimColor);
//    myLights.push_back(keyLight);
//    myLights.push_back(rimLight);
//    // get light step size
//    float bboxSize_x = (finalBBox.max().x() - finalBBox.min().x());
//    float bboxSize_y = (finalBBox.max().y() - finalBBox.min().y());
//    float bboxSize_z = (finalBBox.max().z() - finalBBox.min().z());
//    float bboxSize;
//    bboxSize = getMin(bboxSize_x, bboxSize_y);
//    bboxSize = getMin(bboxSize, bboxSize_z);
//    cout << "	 | BBox size: " << bboxSize << endl;
//    float light_voxelSize = float(bboxSize) / LIGHT_GRID_NUM;
//    float light_step_size = float(light_voxelSize) / CRAZY_NUM;   // get step size form grid
//    cout << "	 | Light voxel size: " << light_voxelSize << endl;
//    cout << "	 | Light step size: " << light_step_size << endl;
//    // get final light volume
//    LightVolume lightVolume(myLights, pyroDensity, pyroMatColor, K, light_step_size, light_voxelSize, finalBBox);
//
//    cout << "Set image..." << endl;
//    Image myImg;
//    myImg.reset(WEIGHT, HEIGHT);
//    cout << "Set camera..." << endl;
//    Camera myCamera;
//    Vector eye(15.0, 15.0, -15.0);
//    Vector view(-1.0, -1.0, 1.0);
//    Vector up(0.0, 1.0, 0.0);
//    myCamera.setFarPlane(NEAR);
//    myCamera.setFarPlane(FAR);
//    myCamera.setEyeViewUp(eye, view, up);
//    cout << "Start rendering..." << endl;
//    cout << "	 | Render step size: " << step_size << endl;
//    Renderer myRenderer(myImg, myCamera, step_size);
//    myRenderer.render(pyroEmColor, pyroDensity, K, lightVolume, finalBBox, 1);
//    cout << "Rendering complete." << endl;
//    // write into file
//    cout << "Write frame " << frame_id << " into" << file_name << "."<< endl;
//    writeOIIOImage(file_name, myImg);
//}

# endif
