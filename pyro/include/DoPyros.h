# ifndef __DOPYROS_H__
# define __DOPYROS_H__

# include <iostream>
# include "omp.h"

# include "Noise.h"
# include "PerlinNoise.h"
# include "Pyroclastic.h"
# include "Camera.h"
# include "Color.h"
# include "Grid.h"
# include "Renderer.h"
# include "OIIOFiles.h"

using namespace std;
using namespace lux;

# define WEIGHT 960
# define HEIGHT 540
# define NEAR 0.1
# define FAR 40
# define CRAZY_NUM 2.0316578
# define LIGHT_GRID_NUM_1 100   // pyro

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

float step_size;


void createPyroWedges(int frame_id, string output_path)
{
    cout << "frame_id: " << frame_id << endl;
    char file_name[1024];
    sprintf(file_name, "%s/jingcoz_hw4.%04d.exr", output_path.c_str(), frame_id);
    cout << "Output path: " << file_name << endl;

    Noise_t parms;
    parms.translate = Vector(0.0, 0.0, frame_id);

    cout << "Noise translate z: " << parms.translate.Z() << endl;

    parms.frequency = 0.57434;
    parms.fjump = 2.6;
    parms.octaves = 4.0;
    parms.gamma = 0.33333;

    BBox finalBBox;
    float K;

    FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    float pyroshpereRadius = 1.0;
    Pyrosphere myPyrosphere(perlin, 1.0);
    BBox pyroBBox = myPyrosphere.getBBox();

    float voxelSize = float(pyroBBox.max().x() - pyroBBox.min().x()) / 500;
    step_size = float(voxelSize) / CRAZY_NUM;   // get step size form grid

    // stamp
    cout << "Stamping pyrosphere..." << endl;
    FloatVolumeToGrid pyroSphereVolume2Grid(myPyrosphere, voxelSize, pyroBBox);
    FloatGrid::Ptr pyroSphereGrid = pyroSphereVolume2Grid.getVolumeGrid();
    BBox pyroNewBBox = pyroSphereVolume2Grid.getBBox();
    cout << "	 | Pyro bounding box: " << pyroNewBBox.min() << " " << pyroNewBBox.max() << endl;
    // gridded
    FloatGridVolume pyroSphereVolume(pyroSphereGrid);

    Color matColor(1.0, 1.0, 1.0, 1.0);
    Color emColor(0.4, 0.0, 0.0, 0.0);
    ConstantFloat rho(7.0);
    ConstantColor pyroMatColor(matColor);
    ConstantColor pyroEmColor(emColor);
    DensityVolume pyroDensity(rho, pyroSphereVolume);

    K = 3;
    finalBBox = pyroNewBBox;

    // lighting
    cout << "Set lights..." << endl;
    Lights myLights;
    // light position
    Vector keyPos(0.0, 4.0, 0.0);
    Vector rimPos(0.0, -4.0, 0.0);
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
    float light_voxelSize = float(bboxSize) / LIGHT_GRID_NUM_1;
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
    Vector eye(8.0, 0.0, 0.0);
    Vector view(-1.0, 0.0, 0.0);
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
