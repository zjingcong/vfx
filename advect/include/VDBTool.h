# ifndef __VDBTOOL_H__
# define __VDBTOOL_H__

# include <iostream>
# include "omp.h"
# include <openvdb/openvdb.h>

# include "Types.h"

using namespace std;


void writeVDBGrid(openvdb::GridPtrVec grids, string vdbFilePath)
{
    double start_time = omp_get_wtime();
    // write grid into file
    // register openvdb types with the system
    openvdb::initialize();
    // create a VDB file object.
    openvdb::io::File file(vdbFilePath);
    // write out the container and close the file
    file.write(grids);
    file.close();
    openvdb::uninitialize();
    double exe_time = omp_get_wtime() - start_time;
    cout << "	 | Elapsed Time: " << exe_time << "s" << endl;
    cout << "Grid file generation complete." << endl;
}


template < typename T >
typename openvdb::Grid<T>::Ptr readVDBGrid(string vdbFilePath, string gridName)
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
    cout << "Generate openVDB grid..." << endl;
    typename openvdb::Grid<T>::Ptr grid = openvdb::gridPtrCast<typename openvdb::Grid<T>>(base);
    double exe_time = omp_get_wtime() - start_time;
    cout << "	 | Elapsed Time: " << exe_time << "s" << endl;

    return grid;
}

# endif
