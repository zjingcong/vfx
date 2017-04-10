
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
    cout << "[Usage] ./bunny cfg_path tag" << endl;
    cout << "\t -l: create bunny levelsets" << endl;
    cout << "\t -p: create pyroclastic bunny" << endl;
    cout << "\t -a: create characteristic map grids" << endl;
    cout << "\t -as [id]: create single characteristic map grid" << endl;
}


int main(int argc, char* argv[])
{
    string tag;
    string cfg_path;
    int cm_id = 0;
    if (argc >= 3)
    {
        cfg_path = argv[1];
        tag = argv[2];
        if (tag != "-p" && tag != "-l" && tag != "-a" && tag != "-as") {printHelp(); exit(0);}
        if (tag == "-as")
        {
            if (argc <= 3)  {cout << "Please input CM id." << endl; printHelp(); exit(0);}
            cm_id = atoi(argv[3]);
        }
    }
    else    {printHelp(); exit(0);}

    setCfgPath(cfg_path);

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

    // create characteristic map grids
    if (tag == "-a")
    {
        cout << "Characteristic Map Grids Generation" << endl;
        cout << "====================================" << endl;
        createCMGrid();
    }

    // create single characteristic map grid
    if (tag == "-as")
    {
        cout << "Single Characteristic Map Grid Generation" << endl;
        cout << "==========================================" << endl;
        createSingleCMGrid(cm_id);
    }

    return 0;
}
