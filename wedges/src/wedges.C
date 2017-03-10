
# include <iostream>

# include "NoiseGrid.h"
# include "Camera.h"
# include "Color.h"
# include "Grid.h"
# include "Renderer.h"
# include "OIIOFiles.h"

using namespace std;
using namespace lux;

# define WEIGHT 960
# define HEIGHT 540
# define STEP_SIZE 0.005
# define LIGHT_STEP_SIZE 0.01
# define NEAR 0.1
# define FAR 40

int main(int argc, char* argv[])
{
    string wedge_type = "noise";
    int frame_id = 0;

    Noise_t parms;
    parms.seed = 485758;
    parms.octaves = 1.0f;
    parms.frequency = 1.0f;
    parms.fjump = 2.0f;
    parms.roughness = 0.5;
    parms.time = 0.0f;
    parms.pscale = 5;

    FractalSum<PerlinNoiseGustavson> perlin;
    perlin.setParameters(parms);

    NoiseGrid perlinNoiseGrid(perlin, 0.02);
    FloatGrid::Ptr perlinGrid = perlinNoiseGrid.getNoiseGrid();
    FloatGridVolume perlinVolume(perlinGrid);

    Vec3s perlinLLC(-5.5, -5.5, -5.5);
    Vec3s perlinURC(5.5, 5.5, 5.5);
    BBox perlinBBox(perlinLLC, perlinURC);

    // -----------------------------------------------------------------------------------
    Color matColor(1.0, 1.0, 1.0, 1.0);
    Color emColor(1.0, 1.0, 1.0, 1.0);
    ConstantColor perlinMatColor(matColor);
    ConstantColor perlinEmColor(emColor);
    ConstantFloat rho(1.0);
    DensityVolume perlinDensity(rho, perlinVolume);
    float K = 0.8;

    // lighting
    cout << "Set lights..." << endl;
    std::vector<LightSource> myLights;
    // light position
    Vector keyPos(0.0, 4.0, 0.0);
    Vector rimPos(0.0, -4.0, 0.0);
    Vector backPos(0.0, 0.0, -4.0);
    // light color
    Color keyColor(0.1, 1.2, 0.53, 1.0);
    Color rimColor(0.34, 0.74, 0.86, 1.0);
    Color backColor(0.016, 0.25, 0.9, 1.0);
    // set lights
    LightSource keyLight(keyPos, keyColor);
    LightSource rimLight(rimPos, rimColor);
    LightSource backLight(backPos, backColor);
    // myLights.push_back(keyLight);
    // myLights.push_back(rimLight);
    // myLights.push_back(backLight);
    LightVolume lightVolume(myLights, perlinDensity, perlinMatColor, K, LIGHT_STEP_SIZE, 0.01, perlinBBox);


    Image myImg;
    myImg.reset(WEIGHT, HEIGHT);
    Camera myCamera;
    Vector eye(20.0, 0.0, 0.0);
    Vector view(-1.0, 0.0, 0.0);
    Vector up(0.0, 1.0, 0.0);
    myCamera.setFarPlane(NEAR);
    myCamera.setFarPlane(FAR);
    myCamera.setEyeViewUp(eye, view, up);
    Renderer myRenderer(myImg, myCamera, STEP_SIZE);
    myRenderer.render(perlinEmColor, perlinVolume, K, lightVolume, perlinBBox, 1);

    // write into file
    char file_name[50];
    sprintf(file_name, "../results/jingcoz_%s_hw3.%04d.exr", wedge_type.c_str(), frame_id);
    cout << "Write frame " << frame_id << " into" << file_name << "."<< endl;
    writeOIIOImage(file_name, myImg);

    return 0;
}
