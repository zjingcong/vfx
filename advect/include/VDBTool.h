# ifndef __VDBTOOL_H__
# define __VDBTOOL_H__

# include <iostream>
# include "omp.h"
# include <openvdb/openvdb.h>

# include "Types.h"

using namespace std;


void writeVDBGrid(openvdb::GridPtrVec grids, string vdbFilePath)
{
    // write grid into file
    // register openvdb types with the system
    openvdb::initialize();
    // create a VDB file object.
    openvdb::io::File file(vdbFilePath);
    // write out the container and close the file
    file.write(grids);
    file.close();
    cout << "Grid file generation complete." << endl;
    openvdb::uninitialize();
}


FloatGrid::Ptr readVDBFloatGrid(string vdbFilePath, string gridName)
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


Vec3fGrid::Ptr readVDBVectorGrid(string vdbFilePath, string gridName)
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
    Vec3fGrid::Ptr grid = openvdb::gridPtrCast<Vec3fGrid>(base);

    return grid;
}

# endif
