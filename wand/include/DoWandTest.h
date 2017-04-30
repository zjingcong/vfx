# ifndef __DOWAND_H__
# define __DOWAND_H__

# include <iostream>
# include <vector>
# include <map>
# include <string>

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
# include "Xform.h"

using namespace std;
using namespace lux;

// constant
# define CRAZY_NUM 2.0316578
// renderer
# define LIGHT_GRID_NUM 50
// voxel size
# define WISP_VOXEL_SIZE 0.03
// color
# define COLOR_ORANGE 0
# define COLOR_PURPLE 1

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


// ------------------------------------------ Filming ------------------------------------------------

// shot lists(360 frames):
// frame 01 - 66: only two wand beams
// frame 67 - 126: boom
// frame 127 - 359: advection


map<int, Color> colorMap;
map<int, FloatGrid::Ptr> wispGridMap;
map<int, Vec4fGrid::Ptr> wispColorGridMap;
vector<VolumeFloatPtr> volumePtrs;
vector<VolumeColorPtr> colorPtrs;
vector<BBox> bboxs;


int start_id = 1;
int start_boom_id = 67;
int start_bunny_id = 127;

void build_colorMap()
{
    colorMap[COLOR_ORANGE] = Color(10.0, 0.8, 2.0, 1.0);
    colorMap[COLOR_PURPLE] = Color(2.0, 0.8, 10.0, 1.0);
}


void do_shot_1(int id)
{
    // ---------------------------------------- do boom -----------------------------------------------

    int total_frame = 60;
    if (id >= 67)
    {
        int boom_id = id - 67;
        for (map<int, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
        {
            int colorName = it->first;
            cout << "Generate grids for color " << colorName << "..." << endl;

            WispParms wispParameters;
            FractalSum<PerlinNoiseGustavson> FSPN1;
            FractalSum<PerlinNoiseGustavson> FSPN2;

            Noise_t FSPN1Parms;
            Noise_t FSPN2Parms;


            float offset = 10.0 * (total_frame - boom_id) / total_frame;

            switch (colorName)
            {
                case COLOR_ORANGE:
                {
                    wispParameters.offset = Vector(0.0, 0.0, offset);
                    FSPN1Parms.translate = Vector(0.0, 0.0, -boom_id * 0.5f);
                    break;
                }

                case COLOR_PURPLE:
                {
                    wispParameters.offset = Vector(0.0, 0.0, -offset);
                    FSPN1Parms.translate = Vector(0.0, 0.0, boom_id * 0.5f);
                    break;
                }

                default:
                    return;
            }

            wispParameters.opacity = 0.008f * boom_id / total_frame + 0.0008f;
            wispParameters.clump = 1.5 * (total_frame - boom_id) / total_frame;
            wispParameters.exp = 1.5 * (total_frame - boom_id) / total_frame + 0.5;
            FSPN2Parms.frequency = 5.5343 + 0.8f * boom_id / total_frame;

            FSPN1Parms.frequency = 0.5325;
            FSPN1Parms.pscale = 1.5;
            FSPN2Parms.octaves = 2.2;
            FSPN2Parms.fjump = 2.5;
            FSPN2Parms.octaves = 2.2;
            FSPN1.setParameters(FSPN1Parms);
            FSPN2.setParameters(FSPN2Parms);

            wispParameters.FSPN1 = &FSPN1;
            wispParameters.FSPN2 = &FSPN2;
            wispParameters.dot_num = 5000000;
            wispParameters.delta_x = 1.5;
            wispParameters.dscale = 2.0;

            Color color = it->second;
            FloatGrid::Ptr wispGrid = wispGridMap[colorName];
            Vec4fGrid::Ptr wispColorGrid = wispColorGridMap[colorName];

            cout << "Stamp grid..." << endl;
            SingleGuideWisp wisp(wispParameters, wispGrid);

            cout << "Stamp color grid..." << endl;
            ConstantColor* colorVolume = new ConstantColor(color);
            ColorVolumeToGrid cyanWispColorV2G(*colorVolume, wispGrid, wispColorGrid);
        }
    }

    // ---------------------------------------- do wand -----------------------------------------------

    float interval = 0.2;
    cout << "Create wisp cloud..." << endl;
    for (map<int, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
    {
        int colorName = it->first;
        Color color = it->second;
        FloatGrid::Ptr wispGrid = wispGridMap[colorName];
        Vec4fGrid::Ptr wispColorGrid = wispColorGridMap[colorName];

        vector<Vector> pathPoints;
        int seed = 0;
        switch (colorName)
        {
            case COLOR_ORANGE:
            {
                Vector p0(0.0, 0.0, 12.0);  pathPoints.push_back(p0);
                Vector p1(0.0, 0.0, -2.0);   pathPoints.push_back(p1);
                seed = 123;
                break;
            }

            case COLOR_PURPLE:
            {
                Vector p0(0.0, 0.0, -12.0);  pathPoints.push_back(p0);
                Vector p1(0.0, 0.0, 2.0);   pathPoints.push_back(p1);
                seed = 456;
                break;
            }

            default:
                return;
        }

        GuidePath path(pathPoints);
        Guideline path_guideline = path.getGuideline();
        cout << "guide num: " << path_guideline.size() << endl;
        cout << "Create wisp cloud for color " << colorName << "..." << endl;
        for (int i = 0; i < path_guideline.size(); ++i)
        {
            if (i > 0)
            {
                Guide p = path_guideline[i];
                Vector start_pos = path_guideline[i-1].pos;
                Vector end_pos = path_guideline[i].pos;
                Vector dir = p.tangent;
                float dp_factor = 0.68;
                int life_time = 26;
                WispCloud wispCloud(interval, start_pos, end_pos, dir, dp_factor, life_time, seed);
                wispCloud.spendTime(id, 0);
                wispCloud.stampWispCloudGrid(wispGrid);
            }
        }

        cout << "Stamp color grid..." << endl;
        ConstantColor* colorVolume = new ConstantColor(color);
        ColorVolumeToGrid cyanWispColorV2G(*colorVolume, wispGrid, wispColorGrid);
    }
}



void createWispLines(int frame_id, string output_path)
{
    /// ----------------------------------- Initialization --------------------------------------------

    cout << "frame_id: " << frame_id << endl;
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw6.%04d.exr", output_path.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;
    cout << "--------------------------------------------" << endl;

    build_colorMap();


    /// ------------------------------------- Grid Setup ----------------------------------------------

    for (map<int, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
    {
        int colorName = it->first;
        Color color = it->second;

        cout << "Create wisp grid for color " << colorName << "..." << endl;
        FloatGrid::Ptr wispGrid = FloatGrid::create(0.0);
        Transform::Ptr xform = wispGrid -> transformPtr();
        xform = Transform::createLinearTransform(WISP_VOXEL_SIZE);
        wispGrid -> setTransform(xform);

        cout << "Create color grid for color " << colorName << "..." << endl;
        Vec4fGrid::Ptr wispColorGrid = Vec4fGrid::create();
        Transform::Ptr colorxform = wispColorGrid -> transformPtr();
        colorxform = Transform::createLinearTransform(WISP_VOXEL_SIZE);
        wispColorGrid -> setTransform(colorxform);

        wispGridMap[colorName] = wispGrid;
        wispColorGridMap[colorName] = wispColorGrid;
    }
    cout << "--------------------------------------------" << endl;

    /// ------------------------------------- Wisp Setup ----------------------------------------------

    int id = frame_id + 1;
    if (id >= start_id && id < start_bunny_id)     { do_shot_1(id); }
    cout << "--------------------------------------------" << endl;

    /// ------------------------------------- Composition ----------------------------------------------

    cout << "Grid to volume..." << endl;
    // grid to volume
    for (map<int, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
    {
        int colorName = it->first;
        FloatGrid::Ptr wispGrid = wispGridMap[colorName];
        Vec4fGrid::Ptr wispColorGrid = wispColorGridMap[colorName];

        VolumeFloatPtr wispVolume = new FloatGridVolume(wispGrid);
        BBox wispBBox = getGridBBox<FloatTree>(wispGrid);
        VolumeColorPtr wispColor = new ColorGridVolume(wispColorGrid);

        volumePtrs.push_back(wispVolume);
        colorPtrs.push_back(wispColor);
        bboxs.push_back(wispBBox);
    }
    cout << "Compositing..." << endl;
    // union float volume
    ImplicitUnionList wispVolume(volumePtrs);
    // union color volume
    ColorAddList wispColor(colorPtrs);
    // union bbox
    BBox wispBBox;
    for (int i = 0; i < bboxs.size(); ++i)
    {
        if (i == 0) {wispBBox = bboxs[0];}
        else
        {
            BBox bbox = bboxs[i];
            wispBBox.expand(bbox);
        }
    }
    cout << "	 | Wisp bounding box: " << wispBBox.min() << " " << wispBBox.max() << endl;
    BBox finalBBox = wispBBox;

    /// ------------------------------------- Property Setup ---------------------------------------------

    // set volume property
    VolumeConstProperty pro = setVolumeConstProperty();
    VolumeFloatPtr finalDensityPtr;
    VolumeColorPtr finalEmColorPtr;
    VolumeColorPtr finalMatColorPtr;
    float K;
    assignVolumeProperty(&wispVolume, pro, finalDensityPtr, finalEmColorPtr, finalMatColorPtr, K);

    finalEmColorPtr = &wispColor;

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
