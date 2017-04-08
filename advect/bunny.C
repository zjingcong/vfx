
# include <iostream>
# include <openvdb/openvdb.h>

# include "Types.h"
# include "VDBTool.h"
# include "PolyModel.h"
# include "Levelsets.h"
# include "DoAdvection.h"

using namespace std;
using namespace lux;

# define VOXEL_SIZE 0.003
# define HALF_NW 80


void printHelp()
{
    cout << "[Usage] ./bunny tag" << endl;
    cout << "\t -l: create bunny levelsets" << endl;
    cout << "\t -p: create pyroclastic bunny" << endl;
}


int main(int argc, char* argv[])
{
    // [usage] ./bunny tag
    // -l: create bunny levelsets
    // -p: create pyroclastic bunny
    string tag;
    if (argc >= 2)
    {
        tag = argv[1];
        if (tag != "-p" && tag != "-l") {printHelp(); exit(0);}
    }
    else    {printHelp(); exit(0);}

    if (tag == "-l")
    {
        cout << "Bunny OpenVDB Levelsets Generation" << endl;
        cout << "====================================" << endl;

        // load bunny model
        string bunnyPath = "../models/cleanbunny.obj";
        // load bunny model
        PolyModel polyBunny;
        polyBunny.loadObj(bunnyPath);
        cout << "--------------------------------------------" << endl;

        // generate bunny levelsets
        cout << "Create levelsets..." << endl;
        cout << "\t | Half Narrow Band: " << VOXEL_SIZE * HALF_NW << endl;
        PolyLevelsets bunnyLevelsets(polyBunny, VOXEL_SIZE, HALF_NW);
        VDBLevelsetsPtr bunnyGrid = bunnyLevelsets.getVDBLevelsets();
        bunnyGrid->setGridClass(openvdb::GRID_LEVEL_SET);
        cout << "--------------------------------------------" << endl;

        // write levelsets grid into file
        writeVDBGrid(bunnyGrid, levelsetsPath, levelsetsGridName);
    }

    if (tag == "-p")
    {
        cout << "Pyroclast Bunny Frame 59 Generation" << endl;
        cout << "====================================" << endl;

        BBox tmp;
        FloatGrid::Ptr bunnyPyroGrid = loadBunnyPyro(59, tmp);
        bunnyPyroGrid->setGridClass(openvdb::GRID_FOG_VOLUME);

        // write pyro grid into file
        writeVDBGrid(bunnyPyroGrid, pyroPath, pyroGridName);
    }

    return 0;
}
