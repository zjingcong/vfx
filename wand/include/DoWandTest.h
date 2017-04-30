# ifndef __DOWAND_H__
# define __DOWAND_H__

# include <iostream>
# include <vector>

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
# include "Particles.h"
# include "Path.h"
# include "DoBoom.h"

using namespace std;
using namespace lux;

// constant
# define CRAZY_NUM 2.0316578
// renderer
# define LIGHT_GRID_NUM 50
// voxel size
# define WISP_VOXEL_SIZE 0.02

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


void createWispLinestest(int frame_id, string output_path)
{
    /// ----------------------------------- Initialization --------------------------------------------

    cout << "frame_id: " << frame_id << endl;
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw6.%04d.exr", output_path.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;
    cout << "--------------------------------------------" << endl;


    /// ------------------------------------- Wisp Setup ----------------------------------------------

    cout << "Create wisp grid..." << endl;
    FloatGrid::Ptr wispGrid = FloatGrid::create(0.0);
    Transform::Ptr xform = wispGrid -> transformPtr();
    xform = Transform::createLinearTransform(WISP_VOXEL_SIZE);
    wispGrid -> setTransform(xform);

    // testBoom(wispGrid);

    float interval = 0.3;

    cout << "Create guide path..." << endl;
    vector<Vector> pathPoints;
    Vector p0(0.0, 0.0, 12.0);  pathPoints.push_back(p0);
    Vector p1(0.0, 0.0, -12.0);   pathPoints.push_back(p1);
    GuidePath path(pathPoints);
    Guideline path_guideline = path.getGuideline();

    cout << "Create wisp cloud..." << endl;
    for (int i = 0; i < path_guideline.size(); ++i)
    {
        if (i > 0)
        {
            Guide p = path_guideline[i];
            Vector start_pos = path_guideline[i-1].pos;
            Vector end_pos = path_guideline[i].pos;
            Vector dir = p.tangent;
            cout << "end_pos: " << end_pos.X() << end_pos.Y() << end_pos.Z() << endl;
            cout << "dir: " << dir.X() << dir.Y() << dir.Z() << endl;
            float dp_factor = 0.68;
            int life_time = 20;
            int seed = 0;
            WispCloud wispCloud(interval, start_pos, end_pos, dir, dp_factor, life_time, seed);
            wispCloud.spendTime(frame_id, 0);
            wispCloud.stampWispCloudGrid(wispGrid);
        }
    }

    // get bbox
    BBox wispBBox = getGridBBox<FloatTree>(wispGrid);
    cout << "	 | Wisp bounding box: " << wispBBox.min() << " " << wispBBox.max() << endl;
    FloatGridVolume wispVolume(wispGrid);
    BBox finalBBox = wispBBox;

    /// ------------------------------------- Property Setup ---------------------------------------------

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
    Vector keyPos(0.0, 10.0, 0.0);
    // Vector rimPos(0.0, -3.0, 3.0);
    // light color
    Color keyColor = Color(5.5, 5.5, 5.5, 1.0);
    // Color rimColor = cfgColorParms.at("cumulorim");
    // set lights
    LightSource keyLight(keyPos, keyColor);
    // LightSource rimLight(rimPos, rimColor);
    // sceneLights.push_back(keyLight);
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
    Vector eye(23.0, 0.0, 0.0);
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
