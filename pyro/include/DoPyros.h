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
# include "Parms.h"

using namespace std;
using namespace lux;

# define WEIGHT 960
# define HEIGHT 540
# define NEAR 0.1
# define FAR 500
# define CRAZY_NUM 2.0316578
# define GRID_NUM 50
# define LIGHT_GRID_NUM 20   // pyro
# define NSPEED 0.1
# define WSPEED 0.1

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

float step_size;


vector<Vector> createTransVecs()
{
    vector<Vector> transVecs;
    float interval_scale = 1.0;
    // generate the square in clockwise order
    // up: 10 pyroclasts(0-9)
    for (int i = 0; i < 10; ++i)
    {
        Vector tmp(-4.5 + i, 0, 4.5);
        transVecs.push_back(tmp * interval_scale);
    }
    // right: 9 pyroclasts(10-18)
    for (int i = 1; i < 10; ++i)
    {
        Vector tmp(4.5, 0, 4.5 - i);
        transVecs.push_back(tmp * interval_scale);
    }
    // bottom: 9 pyroclasts(18-26)
    for (int i = 1; i < 10; ++i)
    {
        Vector tmp(4.5 - i, 0, -4.5);
        transVecs.push_back(tmp * interval_scale);
    }
    // left: 8 pyroclasts(26-35)
    for (int i = 1; i < 9; ++i)
    {
        Vector tmp(-4.5, 0, -4.5 + i);
        transVecs.push_back(tmp * interval_scale);
    }

    return transVecs;
}


Vector getNoiseTrans(int frame_id, int pyro_id)
{
    float trans_scale = NSPEED;
    float trans = frame_id * trans_scale;
    if (pyro_id <= 9)   return Vector(trans, 0, 0);
    else if (pyro_id <= 18) return Vector(0, 0, -trans);
    else if (pyro_id <= 26) return Vector(-trans, 0, 0);
    else    return Vector(0, 0, trans);
}


vector<Noise_t*> createNoiseParmsVecs(int frame_id)
{
    vector<Noise_t*> noiseParmsVecs;
    for (int i = 0; i < 36; ++i)
    {
        Noise_t *parms = new Noise_t;
        parms->translate = getNoiseTrans(frame_id, i);
        parms->octaves = 4.5;
        parms->gamma = 0.43333;
        parms->fjump = fjumpVecs.at(i);
        parms->pscale = pscaleVecs.at(i);
        parms->frequency = freqVecs.at(i);

        noiseParmsVecs.push_back(parms);
    }

    return noiseParmsVecs;
}


void createWave(int frame_id, vector<Noise_t*> noiseParmsVecs)
{
//    int T = 5;  // half wave length
//    float amp_scale = 6.0;
//    for (int i = frame_id - T / 2; i <= frame_id + T / 2; ++i)
//    {
//        int pyroId;
//        pyroId = (i + 36) % 36;
//        float amp = amp_scale * cos(float(i) * M_PI / float(T));
//        noiseParmsVecs.at(pyroId)->amplitude = amp;
//    }
    int T = 9;
    float amp_scale = 6.0;
    float trans_scale = WSPEED;
    for (int i = 0; i < 36; ++i)
    {
        float x = i - frame_id * trans_scale;
        float xx = 2 * M_PI / float(T) * x;
        if (xx <= M_PI)
        {
            float amp = amp_scale * sin(xx);
            amp = (amp > 0) ? amp : 1;
            noiseParmsVecs.at(i)->amplitude = amp;
//            cout << "wave id: " << i << endl;
        }
    }

//    int j = 0;
//    for (Noise_t* n: noiseParmsVecs)
//    {
//        cout << "noise id: " << j << " amp: " << n->amplitude << endl;
//    }
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
    vector<Noise_t*> noiseParmsVecs = createNoiseParmsVecs(frame_id);
    createWave(frame_id, noiseParmsVecs);

    /// ----------------------------------- Pyroclasts Setup ---------------------------------------------

    int i = 0;

    ConstantFloat init(0.0);
    BBox pyroBBox;
    vector<VolumeFloatPtr> pyroVolumePtrs;
    vector<VolumeColorPtr> pyroEmVolumePtrs;

    // generate 36 pyrospheres
    for (int i = 0; i < 36; i++)
    {
        Vector transVec = transVecs.at(i);
        Vec3s transVec3s(transVec.X(), transVec.Y(), transVec.Z());

        FractalSum<PerlinNoiseGustavson> *perlin = new FractalSum<PerlinNoiseGustavson>;
        perlin->setParameters(*noiseParmsVecs.at(i));

        Pyrosphere* pyrosphereOrigin = new Pyrosphere(*perlin);
        BBox pyrosphereBBoxOrigin = pyrosphereOrigin->getBBox();
        // translate
        VolumeFloatPtr pyrosphereTrans = new ScalarTranslate(*pyrosphereOrigin, transVec);
        pyrosphereBBoxOrigin.translate(transVec3s);
        // union bbox
        if (i == 0)  {pyroBBox = pyrosphereBBoxOrigin;}
        else {pyroBBox.expand(pyrosphereBBoxOrigin);}
        pyroVolumePtrs.push_back(pyrosphereTrans);
        // set color for each pyro
        // Color emColor(0.4, 0.0, 0.0, 0.0);
        VolumeColorPtr pyrosphereEm = new ConstantColor(colorVecs.at(i));
        VolumeColorPtr pyrosphereEmVolume = new ColorVolume(*pyrosphereEm, *pyrosphereTrans);
        pyroEmVolumePtrs.push_back(pyrosphereEmVolume);
    }

    // union pyroclasts
    ImplicitUnionList pyro(pyroVolumePtrs);
    VolumeFloatPtr pyroVolumePtr = &pyro;
    // add colors
    ColorAddList pyroEmColorAdd(pyroEmVolumePtrs);

    // get grid voxelSize
    float voxelSize = float(pyroBBox.max().y() - pyroBBox.min().y()) / GRID_NUM;
    step_size = float(voxelSize) / CRAZY_NUM;   // get step size form grid
    // stamp
    cout << "Stamping..." << endl;
    cout << "	 | Grid voxel size: " << voxelSize << endl;
    cout << "Stamping pyro density..." << endl;
    FloatVolumeToGrid pyrosVolume2Grid(*pyroVolumePtr, voxelSize, pyroBBox);
    FloatGrid::Ptr pyrosGrid = pyrosVolume2Grid.getVolumeGrid();
    BBox pyroNewBBox = pyrosVolume2Grid.getBBox();
    cout << "Stamping pyro emcolor..." << endl;
    ColorVolumeToGrid pyroEm2Grid(pyroEmColorAdd, voxelSize, pyroNewBBox);
    Vec4fGrid::Ptr pyroEmGrid = pyroEm2Grid.getVolumeGrid();
    // release memory
    for (VolumeFloatPtr v: pyroVolumePtrs)  { delete v; }
    // gridded
    FloatGridVolume pyrosVolume(pyrosGrid);
    ColorGridVolume pyroEmColor(pyroEmGrid);

    // set matcolor, opacity and density
    Color matColor(1.0, 1.0, 1.0, 1.0);
    ConstantColor pyroMatColor(matColor);
    ConstantFloat rho(7.0);
    DensityVolume pyroDensity(rho, pyrosVolume);
    float K = 3;
    BBox finalBBox = pyroNewBBox;
    cout << "	 | Pyro bounding box: " << finalBBox.min() << " " << finalBBox.max() << endl;

    /// ---------------------------------- Lighting & Rendering -----------------------------------------

    // lighting
    cout << "Set lights..." << endl;
    Lights myLights;
    // light position
    Vector keyPos(0.0, 4.0, 0.0);
    Vector backPos(0.0, -10.0, -10.0);
    // light color
    Color keyColor(0.8, 0.8, 0.8, 1.0);
    Color backColor(0.06, 0.06, 0.06, 1.0);
    // set lights
    LightSource keyLight(keyPos, keyColor);
    LightSource backLight(backPos, backColor);
    myLights.push_back(keyLight);
    myLights.push_back(backLight);
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
    Vector eye(15.0, 15.0, -15.0);
    Vector view(-1.0, -1.0, 1.0);
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
