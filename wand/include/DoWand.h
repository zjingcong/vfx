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
# include "Xform.h"
# include "VDBTool.h"

using namespace std;
using namespace lux;

// constant
# define CRAZY_NUM 2.0316578
// renderer
# define LIGHT_GRID_NUM 80
// voxel size
# define WISP_VOXEL_SIZE 0.03

// camera
# define WEIGHT 860
# define HEIGHT 540
# define NEAR 0.1
# define FAR 500

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

// vdb files
string grid_path;
string levelsetsGridName = "bunny_levelsets";


// ----------------------------------- Volume Property ------------------------------------------------

void assignVolumeProperty(VolumeFloatPtr volume, VolumeConstProperty pro,
                          VolumeFloatPtr& finalDensityPtr, VolumeColorPtr& finalEmColorPtr,
                          VolumeColorPtr& finalMatColorPtr, float& opacity)
{
    // create color volume and density volume
    VolumeColorPtr matColor = new ConstantColor(pro.matColor);
    VolumeColorPtr emColor = new ConstantColor(pro.emColor);
    VolumeFloatPtr rho = new ConstantFloat(pro.rho);
    VolumeFloatPtr density = new DensityVolume(*rho, *volume);
    finalDensityPtr = density;
    finalEmColorPtr = emColor;
    finalMatColorPtr = matColor;
    opacity = pro.K;
}


// ------------------------------------------ Filming ------------------------------------------------

// shot lists(360 frames):
// frame 01 - 51: only two wand beams
// frame 52 - 112: boom
// frame 113 - 360: advection


// map<int, Color> colorMap;
map<int, FloatGrid::Ptr> wispGridMap;
map<int, Vec4fGrid::Ptr> wispColorGridMap;
map<int, Vec4fGrid::Ptr> matColorGridMap;
vector<VolumeFloatPtr> volumePtrs;
vector<VolumeColorPtr> colorPtrs;
vector<VolumeColorPtr> matColorPtrs;
vector<BBox> bboxs;


int start_id = 1;
int start_boom_id = 52;
int start_bunny_id = 113;
int end_id = 360;


void do_shot_1(int id)
{
    // ---------------------------------------- do boom -----------------------------------------------

    int total_frame = start_bunny_id - start_boom_id;   // 60 frame
    if (id >= start_boom_id)
    {
        int boom_id = id - start_boom_id;
        for (map<int, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
        {
            int colorName = it->first;
            if (colorName == COLOR_BUNNY)   {goto endForLoop1;}
            cout << "Generate grids for color " << colorName << "..." << endl;

            WispParms wispParameters;
            FractalSum<PerlinNoiseGustavson> FSPN1;
            FractalSum<PerlinNoiseGustavson> FSPN2;

            Noise_t FSPN1Parms;
            Noise_t FSPN2Parms;


            float boom_factor;
            if (id >= start_bunny_id)   {boom_factor = 0.0;}
            else (boom_factor = (total_frame - boom_id) / total_frame);

            float offset = 10.0 *  boom_factor;
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

            wispParameters.opacity = 0.008f * (1.0f -  boom_factor) + 0.0008f;
            wispParameters.clump = 1.5 *  boom_factor;
            wispParameters.exp = 1.5 *  boom_factor + 0.5;
            FSPN2Parms.frequency = 5.5343 + 0.8f * (1.0f -  boom_factor);

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
        endForLoop1: ;
    }

    // ---------------------------------------- do wand -----------------------------------------------

    float interval = 0.3;
    cout << "Create wisp cloud..." << endl;
    for (map<int, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
    {
        int colorName = it->first;
        if (colorName == COLOR_BUNNY)   {goto endForLoop2;}
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
                Vector p1(0.0, 0.0, 0.0);   pathPoints.push_back(p1);
                seed = 123;
                break;
            }

            case COLOR_PURPLE:
            {
                Vector p0(0.0, 0.0, -12.0);  pathPoints.push_back(p0);
                Vector p1(0.0, 0.0, 0.0);   pathPoints.push_back(p1);
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
    endForLoop2: ;

    cout << "--------------------------------------------" << endl;
}


void do_shot_2(int id)
{
    // ---------------------------------------- do boom -----------------------------------------------

//    int total_frame = start_bunny_id - start_boom_id;   // 60 frame
//    if (id >= start_bunny_id)
//    {
//        int boom_id = id - start_boom_id;
//        for (map<int, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
//        {
//            int colorName = it->first;
//            if (colorName == COLOR_BUNNY)   {goto endForLoop1;}
//            cout << "Generate grids for color " << colorName << "..." << endl;
//
//            WispParms wispParameters;
//            FractalSum<PerlinNoiseGustavson> FSPN1;
//            FractalSum<PerlinNoiseGustavson> FSPN2;
//
//            Noise_t FSPN1Parms;
//            Noise_t FSPN2Parms;
//
//            float boom_factor;
//            if (id >= start_bunny_id)   {boom_factor = 0.0;}
//            else (boom_factor = (total_frame - boom_id) / total_frame);
//
//            float offset = 10.0 *  boom_factor;
//            switch (colorName)
//            {
//                case COLOR_ORANGE:
//                {
//                    wispParameters.offset = Vector(0.0, 0.0, offset);
//                    FSPN1Parms.translate = Vector(0.0, 0.0, -boom_id * 0.5f);
//                    break;
//                }
//
//                case COLOR_PURPLE:
//                {
//                    wispParameters.offset = Vector(0.0, 0.0, -offset);
//                    FSPN1Parms.translate = Vector(0.0, 0.0, boom_id * 0.5f);
//                    break;
//                }
//
//                default:
//                    return;
//            }
//
//            wispParameters.opacity = 0.008f * (1.0f -  boom_factor) + 0.0008f;
//            wispParameters.clump = 1.5 *  boom_factor;
//            wispParameters.exp = 1.5 *  boom_factor + 0.5;
//            FSPN2Parms.frequency = 5.5343 + 0.8f * (1.0f -  boom_factor);
//
//            FSPN1Parms.frequency = 0.5325;
//            FSPN1Parms.pscale = 1.5;
//            FSPN2Parms.octaves = 2.2;
//            FSPN2Parms.fjump = 2.5;
//            FSPN2Parms.octaves = 2.2;
//            FSPN1.setParameters(FSPN1Parms);
//            FSPN2.setParameters(FSPN2Parms);
//
//            wispParameters.FSPN1 = &FSPN1;
//            wispParameters.FSPN2 = &FSPN2;
//            wispParameters.dot_num = 5000000;
//            wispParameters.delta_x = 1.5;
//            wispParameters.dscale = 2.0;
//
//            Color color = it->second;
//            FloatGrid::Ptr wispGrid = wispGridMap[colorName];
//            Vec4fGrid::Ptr wispColorGrid = wispColorGridMap[colorName];
//
//            cout << "Stamp grid..." << endl;
//            SingleGuideWisp wisp(wispParameters, wispGrid);
//
//            cout << "Stamp color grid..." << endl;
//            ConstantColor* colorVolume = new ConstantColor(color);
//            ColorVolumeToGrid cyanWispColorV2G(*colorVolume, wispGrid, wispColorGrid);
//        }
//        endForLoop1: ;
//    }

    // load bunny levelsets from .vdb file
    string levelsetsPath = grid_path + "/bunny_levelsets.vdb";
    cout << "Load bunny levelsets " << levelsetsGridName << " from .vdb file " << levelsetsPath << endl;
    FloatGrid::Ptr bunnyLSGrid = readVDBGrid<FloatTree>(levelsetsPath, levelsetsGridName);
    BBox bunnyLevelsetsBBox = getGridBBox<FloatTree>(bunnyLSGrid);
    cout << "\t | Bunny Levelsets BBox: " << bunnyLevelsetsBBox.min() << " " << bunnyLevelsetsBBox.max() << endl;
    Transform::Ptr bunnyXform = bunnyLSGrid->transformPtr();
    // rotate
    bunnyXform->postRotate(3 * M_PI / 4.0, openvdb::math::Y_AXIS);
    bunnyLSGrid->setTransform(bunnyXform);
    // scale
    bunnyXform->postScale(3.0);
    bunnyLSGrid->setTransform(bunnyXform);

    VolumeFloatPtr bunnyVolume = new VDBLevelsetsVolume(bunnyLSGrid);
    BBox temp(Vec3s(-4, -4, -4), Vec3s(4, 4, 4));
    FloatVolumeToGrid bunnyGridV2G(*bunnyVolume, WISP_VOXEL_SIZE, temp);
    FloatGrid::Ptr bunnyGrid = bunnyGridV2G.getVolumeGrid();
    BBox bunnyBBox = getGridBBox<FloatTree>(bunnyGrid);
    cout << bunnyBBox.min() << bunnyBBox.max() << endl;
    wispGridMap[COLOR_BUNNY] = bunnyGrid;

    cout << "Stamp color grid..." << endl;
    Color bunnycolor = colorMap[COLOR_BUNNY];
    Vec4fGrid::Ptr bunnyMatColorGrid = matColorGridMap[COLOR_BUNNY];
    VolumeColorPtr bunnyConstColorVolume = new ConstantColor(bunnycolor);
    VolumeColorPtr bunnyColorVolume = new ColorVolume(*bunnyConstColorVolume, *bunnyVolume);
    ColorVolumeToGrid bunnyColorV2G(*bunnyColorVolume, WISP_VOXEL_SIZE, temp);
    matColorGridMap[COLOR_BUNNY] = bunnyColorV2G.getVolumeGrid();
}


void createCMGrid()
{
    // get advect parms config
    cout << "Get advection parms..." << endl;
    cfg::FloatValueMap cfgAdvectParms;
    cfgAdvectParms = cfg::floatValueParser(advectConfigPath);

    cout << "Create perlin noise..." << endl;
    // set noise parms
    Noise_t parms;
    parms.gamma = 0.333;
    parms.frequency = 10.3527;
    parms.fjump = 2.0;
    parms.octaves = 0.5;
    // create perlin noise
    FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    float timestep = 0.0008;
    int stepnum = 20;
    float offset = 0.005;

    // generatePyro velocity field from vector noise
    cout << "Create velocity field..." << endl;
    Vector delta_x = offset * Vector(1.0, 1.0, 1.0);
    Vector delta_y = offset * Vector(1.0, 0.0, -1.0);
    Vector delta_z = offset * Vector(0.0, 1.0, 0.0);
    VNoise1 velocityField(perlin, delta_x, delta_y, delta_z);

    // initialization
    Identity identity;
    VectorGridVolumePtr X;
    BBox advectBBox(Vec3s(-3.0, -3.0, -3.0), Vec3s(3.0, 3.0, 3.0));
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

        // generatePyro characteristic map grid
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


void createWispLines(int frame_id, string output_path, string grid_p)
{
    /// ----------------------------------- Initialization --------------------------------------------

    cout << "frame_id: " << frame_id << endl;
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw6.%04d.exr", output_path.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;
    grid_path = grid_p;
    cout << "Grid path: " << grid_path << endl;
    cout << "--------------------------------------------" << endl;

    build_colorMap();

    /// ------------------------------------- Grid Setup ----------------------------------------------

    for (map<int, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
    {
        int colorName = it->first;
        FloatGrid::Ptr wispGrid;

        cout << "Create wisp grid for color " << colorName << "..." << endl;
        wispGrid = FloatGrid::create(0.0);
        Transform::Ptr xform = wispGrid -> transformPtr();
        xform = Transform::createLinearTransform(WISP_VOXEL_SIZE);
        wispGrid -> setTransform(xform);

        Color color = it->second;
        cout << "Create color grid for color " << colorName << "..." << endl;
        Vec4fGrid::Ptr wispColorGrid = Vec4fGrid::create();
        Transform::Ptr colorxform = wispColorGrid -> transformPtr();
        colorxform = Transform::createLinearTransform(WISP_VOXEL_SIZE);
        wispColorGrid -> setTransform(colorxform);

        cout << "Create matcolor grid for color " << colorName << "..." << endl;
        Vec4fGrid::Ptr matColorGrid = Vec4fGrid::create();
        Transform::Ptr matcolorxform = matColorGrid -> transformPtr();
        matcolorxform = Transform::createLinearTransform(WISP_VOXEL_SIZE);
        matColorGrid -> setTransform(matcolorxform);

        wispGridMap[colorName] = wispGrid;
        wispColorGridMap[colorName] = wispColorGrid;
        matColorGridMap[colorName] = matColorGrid;
    }
    cout << "--------------------------------------------" << endl;

    /// ------------------------------------- Wisp Setup ----------------------------------------------

    cout << "Setup wisp scene..." << endl;
    int id = frame_id + 1;
    if (id >= start_id && id < start_bunny_id)     {do_shot_1(id);}
    else if (id >= start_bunny_id && id <= end_id)   {do_shot_2(id);}
    cout << "--------------------------------------------" << endl;

    /// ------------------------------------- Composition ----------------------------------------------

    cout << "Grid to volume..." << endl;
    // grid to volume
    for (map<int, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
    {
        int colorName = it->first;
        FloatGrid::Ptr wispGrid = wispGridMap[colorName];
        Vec4fGrid::Ptr wispColorGrid = wispColorGridMap[colorName];
        Vec4fGrid::Ptr matColorGrid = matColorGridMap[colorName];

        VolumeFloatPtr wispVolume;
//        if (colorName != COLOR_BUNNY)
            {wispVolume = new FloatGridVolume(wispGrid);}
//        else
//            {wispVolume = new VDBLevelsetsVolume(wispGrid);}
        BBox wispBBox = getGridBBox<FloatTree>(wispGrid);
        VolumeColorPtr wispColor = new ColorGridVolume(wispColorGrid);
        VolumeColorPtr matColor = new ColorGridVolume(matColorGrid);

        // set volume property
        VolumeConstProperty pro = setVolumeConstProperty();
        VolumeConstProperty bunnypro = setBunnyConstProperty();
        VolumeFloatPtr densityPtr;
        VolumeColorPtr emColorPtr;
        VolumeColorPtr matColorPtr;
        float K;
        if (colorName == COLOR_BUNNY)
        {
            assignVolumeProperty(wispVolume, bunnypro, densityPtr, emColorPtr, matColorPtr, K);
            matColorPtr = matColor;
        }
        else
        {
            assignVolumeProperty(wispVolume, pro, densityPtr, emColorPtr, matColorPtr, K);
            emColorPtr = wispColor;
        }

        volumePtrs.push_back(densityPtr);
        colorPtrs.push_back(emColorPtr);
        matColorPtrs.push_back(matColorPtr);
        bboxs.push_back(wispBBox);
    }

    cout << "Compositing..." << endl;
    // union float volume
    ImplicitUnionList wispVolume(volumePtrs);
    // union emcolor volume
    ColorAddList wispColor(colorPtrs);
    // union matcolor volume
    ColorAddList matColor(matColorPtrs);
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
    VolumeFloatPtr finalDensityPtr = &wispVolume;
    VolumeColorPtr finalEmColorPtr = &wispColor;
    VolumeColorPtr finalMatColorPtr = &matColor;
    float K = 0.4;

    /// ---------------------------------- Lighting & Rendering ---------------------------------------

    float step_size = WISP_VOXEL_SIZE / CRAZY_NUM;   // get step size form grid

    // lighting
    cout << "Set lights..." << endl;
    Lights sceneLights;
    // light position
    Vector keyPos(0.0, 10.0, 0.0);
    // Vector rimPos(0.0, -3.0, 3.0);
    // light color
    Color keyColor = Color(0.2, 0.2, 0.2, 1.0);
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
