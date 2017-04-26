# ifndef __DOWANDTEST_H__
# define __DOWANDTEST_H__

# include <iostream>

# include "Types.h"
# include "Parms.h"
# include "Camera.h"
# include "Color.h"
# include "Renderer.h"
# include "OIIOFiles.h"
# include "Vector.h"
# include "Noise.h"
# include "PerlinNoise.h"
# include "Wisp.h"

using namespace std;
using namespace lux;

// constant
# define CRAZY_NUM 2.0316578
// renderer
# define LIGHT_GRID_NUM 50
// voxel size
# define WISP_VOXEL_SIZE 0.015

// camera
# define WEIGHT 860
# define HEIGHT 540
# define NEAR 0.1
# define FAR 500

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)


// ----------------------------------- Volume Property ------------------------------------------------

void assignVolumeProperty(VolumeFloatPtr volume, VolumeConstProperty pro,
                          VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
                          VolumeColorPtr& finalMatColorPtr, float& opacity)
{
    // create color volume and density volume
    static ConstantColor matColor(pro.matColor);
    static ConstantColor emColor(pro.emColor);
    static ConstantFloat rho(pro.rho);
    static DensityVolume density(rho, *volume);
    finalDensityPtr = &density;
    finalEmColorPtr = &emColor;
    finalMatColorPtr = &matColor;
    opacity = pro.K;
}


// ------------------------------------------ rendering ------------------------------------------------


void testCreateWispLines(int frame_id, string output_path)
{
    /// ----------------------------------- Initialization --------------------------------------------

    cout << "frame_id: " << frame_id << endl;
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw6.%04d.exr", output_path.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;
    cout << "--------------------------------------------" << endl;


    /// ------------------------------------- Wisp Setup ----------------------------------------------

    FractalSum<PerlinNoiseGustavson> FSPN1;
    FractalSum<PerlinNoiseGustavson> FSPN2;

    Noise_t FSPN1Parms;
    Noise_t FSPN2Parms;
    // id 383
    FSPN2Parms.octaves = 1.9;
    FSPN2Parms.frequency = 1.5343;
    FSPN2Parms.fjump = 2.5;
    // FSPN2Parms.translate = Vector(0.0, frame_id * 0.1, 0.0);

    // FSPN1Parms.frequency = 1.52032;
    FSPN1Parms.translate = Vector(0.0, frame_id * 0.1, 0.0);

    FSPN1.setParameters(FSPN1Parms);
    FSPN2.setParameters(FSPN2Parms);

    WispParms wispParameters;
    wispParameters.clump = 2.0;
    wispParameters.FSPN1 = &FSPN1;
    wispParameters.FSPN2 = &FSPN2;
    wispParameters.dot_num = 5000000;
    wispParameters.offset = Vector(0.0, 0.0, 6.0);

    cout << "Create wisp grid..." << endl;
    SingleGuideWisp wisp(wispParameters, WISP_VOXEL_SIZE);
    FloatGrid::Ptr wispGrid = wisp.getWispGrid();
    BBox wispBBox = wisp.getBBox();
    cout << "	 | Wisp bounding box: " << wispBBox.min() << " " << wispBBox.max() << endl;
    FloatGridVolume wispVolume(wispGrid);

    BBox finalBBox = wispBBox;

    // set volume property
    VolumeConstProperty pro = setVolumeConstProperty();
    VolumeFloatPtr finalDensityPtr;
    VolumeColorPtr finalEmColorPtr;
    VolumeColorPtr finalMatColorPtr;
    float K;
    assignVolumeProperty(&wispVolume, pro, finalDensityPtr, finalEmColorPtr, finalMatColorPtr, K);

    /// ---------------------------------- Lighting & Rendering ---------------------------------------

    float step_size = WISP_VOXEL_SIZE / CRAZY_NUM;   // get step size form grid

    // lighting
    cout << "Set lights..." << endl;
    Lights sceneLights;
    // light position
    Vector keyPos(0.0, 5.0, 0.0);
    // Vector rimPos(0.0, -3.0, 3.0);
    // light color
    Color keyColor = Color(1.5, 1.5, 1.5, 1.0);
    // Color rimColor = cfgColorParms.at("cumulorim");
    // set lights
    LightSource keyLight(keyPos, keyColor);
    // LightSource rimLight(rimPos, rimColor);
    sceneLights.push_back(keyLight);
    // myLights.push_back(rimLight);
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
    LightVolume lightVolume(sceneLights, *finalDensityPtr, *finalMatColorPtr, K, light_step_size, light_voxelSize, finalBBox);

    cout << "Set image..." << endl;
    Image myImg;
    myImg.reset(WEIGHT, HEIGHT);
    cout << "Set camera..." << endl;
    Camera myCamera;
    Vector eye(10.0, 0.0, 0.0);
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
