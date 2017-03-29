# ifndef __DOPYROS_H__
# define __DOPYROS_H__

# include <iostream>
# include <vector>
# include "omp.h"
# include <openvdb/tools/Composite.h>

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

# define WEIGHT 960
# define HEIGHT 540
# define NEAR 0.1
# define FAR 500
# define CRAZY_NUM 2.0316578
# define GRID_NUM 25
# define LIGHT_GRID_NUM 20   // pyro

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

float step_size;


vector<Vector> createTransVecs()
{
    vector<Vector> transVecs;
    float interval_scale = 3.0;
    // generate the square in clockwise order
    // up: 10 pyroclasts
    for (int i = 0; i < 10; ++i)
    {
        Vector tmp(-4.5 + i, 0, 4.5);
        transVecs.push_back(tmp * interval_scale);
    }
    // right: 9 pyroclasts
    for (int i = 1; i < 10; ++i)
    {
        Vector tmp(4.5, 0, 4.5 - i);
        transVecs.push_back(tmp * interval_scale);
    }
    // bottom: 9 pyroclasts
    for (int i = 1; i < 10; ++i)
    {
        Vector tmp(4.5 - i, 0, -4.5);
        transVecs.push_back(tmp * interval_scale);
    }
    // left: 8 pyroclasts
    for (int i = 1; i < 9; ++i)
    {
        Vector tmp(-4.5, 0, -4.5 + i);
        transVecs.push_back(tmp * interval_scale);
    }

    return transVecs;
}


vector<Noise_t> createNoiseParmsVecs(int frame_id)
{
    vector<Noise_t> noiseParmsVecs;
    for (int i = 0; i < 36; ++i)
    {
        Noise_t parms;
        parms.translate = Vector(0.0, 0.0, frame_id);
        parms.frequency = 0.57434;
        parms.fjump = 2.6;
        parms.octaves = 4.0;
        parms.gamma = 0.33333;
        parms.pscale = 1.0;

        noiseParmsVecs.push_back(parms);
    }

    for (Noise_t n: noiseParmsVecs)
    {
        n.gamma = 1.0;
    }

    return noiseParmsVecs;
}


void createPyroWedges(int frame_id, string output_path)
{
    /// ----------------------------------- Initialization ---------------------------------------------
    cout << "frame_id: " << frame_id << endl;
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw4.%04d.exr", output_path.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;

    // get parms for pyrolist
    vector<Vector> transVecs = createTransVecs();
    vector<Noise_t> noiseParmsVecs = createNoiseParmsVecs(frame_id);

    /// ----------------------------------- Pyroclasts Setup ---------------------------------------------

    int i = 0;

    ConstantFloat init(0.0);
    BBox pyroBBox;
    vector<VolumeFloatPtr> pyroVolumePtrs;
    vector<BBox> bboxs;

    // generate 36 pyrospheres
    for (int i = 0; i < 36; i++)
    {
        Vector transVec = transVecs.at(i);
        Vec3s transVec3s(transVec.X(), transVec.Y(), transVec.Z());

        Noise_t *parms = new Noise_t;
        *parms = noiseParmsVecs.at(i);
        FractalSum<PerlinNoiseGustavson> *perlin = new FractalSum<PerlinNoiseGustavson>;
        perlin->setParameters(*parms);

        Pyrosphere pyrosphereOrigin(*perlin);
        BBox pyrosphereBBoxOrigin = pyrosphereOrigin.getBBox();
        // translate
        VolumeFloatPtr pyrosphereTrans = new ScalarTranslate(pyrosphereOrigin, transVec);
        pyrosphereBBoxOrigin.translate(transVec3s);
        bboxs.push_back(pyrosphereBBoxOrigin);
        // union
        if (i == 0)  {pyroBBox = pyrosphereBBoxOrigin;}
        else {pyroBBox.expand(pyrosphereBBoxOrigin);}
        pyroVolumePtrs.push_back(pyrosphereTrans);
    }

    // union pyroclasts
    ImplicitUnionList pyro(pyroVolumePtrs);
    VolumeFloatPtr pyroVolumePtr = &pyro;

    // get grid voxelSize
    float voxelSize = float(pyroBBox.max().y() - pyroBBox.min().y()) / GRID_NUM;
    step_size = float(voxelSize) / CRAZY_NUM;   // get step size form grid
    // stamp
    cout << "Stamping pyrosphere..." << endl;
    cout << "	 | Grid voxel size: " << voxelSize << endl;
    FloatVolumeToGrid pyrosVolume2Grid(*pyroVolumePtr, voxelSize, pyroBBox);
    FloatGrid::Ptr pyrosGrid = pyrosVolume2Grid.getVolumeGrid();
    BBox pyroNewBBox = pyrosVolume2Grid.getBBox();
    // release memory
    for (VolumeFloatPtr v: pyroVolumePtrs)  { delete v; }
    // gridded
    FloatGridVolume pyrosVolume(pyrosGrid);

    // set color, opacity and density
    Color matColor(1.0, 1.0, 1.0, 1.0);
    Color emColor(0.4, 0.0, 0.0, 0.0);
    ConstantFloat rho(7.0);
    ConstantColor pyroMatColor(matColor);
    ConstantColor pyroEmColor(emColor);
    DensityVolume pyroDensity(rho, pyrosVolume);
    float K = 3;
    BBox finalBBox = pyroNewBBox;
    cout << "	 | Pyro bounding box: " << finalBBox.min() << " " << finalBBox.max() << endl;

    /// ---------------------------------- Lighting & Rendering -----------------------------------------

    // lighting
    cout << "Set lights..." << endl;
    Lights myLights;
    // light position
    Vector keyPos(10.0, 10.0, 0.0);
    Vector rimPos(-10.0, -10.0, 0.0);
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
    float light_voxelSize = float(bboxSize) / LIGHT_GRID_NUM;
    float light_step_size = float(light_voxelSize) / CRAZY_NUM;   // get step size form grid
    cout << "	 | Light voxel size: " << light_voxelSize << endl;
    cout << "	 | Light step size: " << light_step_size << endl;
    // get final light volume
    LightVolume lightVolume(myLights, pyroDensity, pyroMatColor, K, light_step_size, light_voxelSize, finalBBox);

    cout << "Set image..." << endl;
    Image myImg;
    myImg.reset(WEIGHT, HEIGHT);
    cout << "Set camera..." << endl;
    Camera myCamera;
    Vector eye(70.0, 70.0, 0.0);
    Vector view(-1.0, -1.0, 0.0);
    Vector up(0.0, 1.0, 0.0);
    myCamera.setFarPlane(NEAR);
    myCamera.setFarPlane(FAR);
    myCamera.setEyeViewUp(eye, view, up);
    cout << "Start rendering..." << endl;
    cout << "	 | Render step size: " << step_size << endl;
    Renderer myRenderer(myImg, myCamera, step_size);
    myRenderer.render(pyroEmColor, pyroDensity, K, lightVolume, finalBBox, 1);
    cout << "Rendering complete." << endl;
    // write into file
    cout << "Write frame " << frame_id << " into" << file_name << "."<< endl;
    writeOIIOImage(file_name, myImg);
}

# endif
