
# include "DoWedges.h"

using namespace std;
using namespace lux;

# define WEIGHT 960
# define HEIGHT 540
# define STEP_SIZE 0.005
# define LIGHT_STEP_SIZE 0.01
# define NEAR 0.1
# define FAR 40

VolumeFloatPtr finalDensityPtr;
VolumeColorPtr finalEmColorPtr;
VolumeColorPtr finalMatColorPtr;
BBox finalBBox;
float K;


int main(int argc, char* argv[])
{
    string wedge_type = "noise";
    int frame_id = 0;

    noiseWedges(finalDensityPtr, finalEmColorPtr, finalMatColorPtr, finalBBox, K);

    // lighting
    cout << "Set lights..." << endl;
    Lights myLights;
    // light position
    Vector keyPos(0.0, 4.0, 0.0);
    Vector rimPos(0.0, -4.0, 0.0);
    // light color
    Color keyColor(0.1, 1.2, 0.53, 1.0);
    Color rimColor(0.34, 0.74, 0.86, 1.0);
    // set lights
    LightSource keyLight(keyPos, keyColor);
    LightSource rimLight(rimPos, rimColor);
    myLights.push_back(keyLight);
    myLights.push_back(rimLight);
    LightVolume lightVolume(myLights, *finalDensityPtr, *finalMatColorPtr, K, LIGHT_STEP_SIZE, 0.1, finalBBox);

    VolumeColorPtr lightPtr;
    lightPtr = &lightVolume;

    cout << "Set image..." << endl;
    Image myImg;
    myImg.reset(WEIGHT, HEIGHT);
    cout << "Set camera..." << endl;
    Camera myCamera;
    Vector eye(20.0, 0.0, 0.0);
    Vector view(-1.0, 0.0, 0.0);
    Vector up(0.0, 1.0, 0.0);
    myCamera.setFarPlane(NEAR);
    myCamera.setFarPlane(FAR);
    myCamera.setEyeViewUp(eye, view, up);
    cout << "Start rendering..." << endl;
    Renderer myRenderer(myImg, myCamera, STEP_SIZE);
    myRenderer.render(*finalEmColorPtr, *finalDensityPtr, K, *lightPtr, finalBBox, 1);
    cout << "Rendering complete." << endl;
    // write into file
    char file_name[50];
    sprintf(file_name, "../results/jingcoz_hw3_%s.%04d.exr", wedge_type.c_str(), frame_id);
    cout << "Write frame " << frame_id << " into" << file_name << "."<< endl;
    writeOIIOImage(file_name, myImg);

    return 0;
}
