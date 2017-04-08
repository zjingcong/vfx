
# include <iostream>
# include <openvdb/openvdb.h>

# include "Types.h"
# include "PolyModel.h"
# include "Levelsets.h"

using namespace std;
using namespace lux;

# define VOXEL_SIZE 0.003
# define HALF_NW 80

string levelsetsPath = "../tmp/bunny_levelsets.vdb";
string gridName = "bunny_levelsets";


int main()
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
    cout << "--------------------------------------------" << endl;
    
    // write grid into file
    // register openvdb types with the system
    openvdb::initialize();
    cout << "Writing levelsets into file: " << levelsetsPath << "..." << endl;
    bunnyGrid->setGridClass(openvdb::GRID_LEVEL_SET);
    bunnyGrid->setName(gridName);
    cout << "\t | Grid name: " << bunnyGrid->getName() << endl;
    // create a VDB file object.
    openvdb::io::File file(levelsetsPath);
    openvdb::GridPtrVec grids;
    grids.push_back(bunnyGrid);
    // write out the container and close the file
    file.write(grids);
    file.close();
    cout << "Levelsets file generation complete." << endl;
    openvdb::uninitialize();

    return 0;
}
