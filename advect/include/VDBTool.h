# ifndef __VDBTOOL_H__
# define __VDBTOOL_H__

# include <iostream>
# include "omp.h"
# include <openvdb/openvdb.h>

# include "Types.h"

using namespace std;


void writeVDBGrid(FloatGrid::Ptr grid, string vdbFilePath, string gridName)
{
    // write grid into file
    // register openvdb types with the system
    openvdb::initialize();
    cout << "Writing grid " << gridName << " into file: " << vdbFilePath << "..." << endl;
    grid->setName(gridName);
    // create a VDB file object.
    openvdb::io::File file(vdbFilePath);
    openvdb::GridPtrVec grids;
    grids.push_back(grid);
    // write out the container and close the file
    file.write(grids);
    file.close();
    cout << "Grid file generation complete." << endl;
    openvdb::uninitialize();
}


FloatGrid::Ptr readVDBGrid(string vdbFilePath, string gridName)
{
    cout << "Load openVDB grid " << gridName << " from .vdb file " << vdbFilePath << endl;
    double start_time = omp_get_wtime();
    openvdb::initialize();
    openvdb::io::File fileR(vdbFilePath);
    fileR.open();
    openvdb::GridBase::Ptr base;
    for (openvdb::io::File::NameIterator iter = fileR.beginName(); iter != fileR.endName(); ++iter)
    {
        if (iter.gridName() == gridName)    {base = fileR.readGrid(iter.gridName());}
    }
    fileR.close();
    openvdb::uninitialize();
    double exe_time = omp_get_wtime() - start_time;
    cout << "	 | Elapsed Time: " << exe_time << "s" << endl;

    cout << "Generate openVDB grid..." << endl;
    FloatGrid::Ptr grid = openvdb::gridPtrCast<FloatGrid>(base);

    return grid;
}

# endif
