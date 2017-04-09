
# include <iostream>
# include <openvdb/openvdb.h>

# include "Types.h"
# include "VDBTool.h"
# include "PolyModel.h"
# include "Levelsets.h"
# include "DoAdvection.h"

using namespace std;
using namespace lux;


void printHelp()
{
    cout << "[Usage] ./bunny tag" << endl;
    cout << "\t -l: create bunny levelsets" << endl;
    cout << "\t -p: create pyroclastic bunny" << endl;
    cout << "\t -a: create characteristic map grid" << endl;
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
        if (tag != "-p" && tag != "-l" && tag != "-a") {printHelp(); exit(0);}
    }
    else    {printHelp(); exit(0);}

    // create levelsets grid
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
        cout << "Writing grid " << levelsetsGridName << " into file: " << levelsetsPath << "..." << endl;
        bunnyGrid->setName(levelsetsGridName);
        openvdb::GridPtrVec grids;
        grids.push_back(bunnyGrid);
        writeVDBGrid(grids, levelsetsPath);
    }

    // create pyroclast grid
    if (tag == "-p")
    {
        cout << "Pyroclast Bunny Frame 59 Generation" << endl;
        cout << "====================================" << endl;

        BBox tmp;
        FloatGrid::Ptr bunnyPyroGrid = loadBunnyPyro(59, tmp);
        bunnyPyroGrid->setGridClass(openvdb::GRID_FOG_VOLUME);

        // write pyro grid into file
        cout << "Writing grid " << pyroGridName << " into file: " << pyroPath << "..." << endl;
        bunnyPyroGrid->setName(pyroGridName);
        openvdb::GridPtrVec grids;
        grids.push_back(bunnyPyroGrid);
        writeVDBGrid(grids, pyroPath);
    }

    // create characteristic map grid
    if (tag == "-a")
    {
        cout << "Characteristic Map Grids Generation" << endl;
        cout << "====================================" << endl;
        createCMGrid();
    }

    return 0;
}
