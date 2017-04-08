# include <vector>
# include <iostream>
# include <openvdb/tools/MeshToVolume.h>

# include "Levelsets.h"

using namespace lux;


BBox PolyLevelsets::getBBox()
{
	levelsetsBBox = getGridBBox(VDBLevelsetsGrid);

	return levelsetsBBox;
}


FloatGrid::Ptr PolyLevelsets::VDBcreateLevelsets()
{
	std::vector<Vec3s> points = polyModel.polyPoints;
	std::vector<Vec3I> triangles = polyModel.triIndices;
	Transform::Ptr lXform = Transform::createLinearTransform(voxelSize);
	FloatGrid::Ptr mesh2levelsets = openvdb::tools::meshToLevelSet<FloatGrid>(*lXform, points, triangles, halfWidth);
	// get background value
	backgroundValue = mesh2levelsets->background();
	cout << "\t | VDB Levelsets Background: " << backgroundValue << endl;

	return mesh2levelsets;
}


// return the polymodel levelsets
FloatGrid::Ptr PolyLevelsets::getLevelsets()
{
	FloatGrid::Ptr mesh2levelsets = VDBcreateLevelsets();
	VDBLevelsetsGrid = mesh2levelsets;
	levelsetsBBox = getGridBBox(VDBLevelsetsGrid);
	cout << "Convert VDB levelsets..." << endl;
	VDBLevelsetsVolume levelsetsVolume(mesh2levelsets);
	FloatVolumeToGrid levelsetsVolume2Grid(levelsetsVolume, voxelSize, levelsetsBBox);
	FloatGrid::Ptr levelsetsGrid = levelsetsVolume2Grid.getVolumeGrid(-backgroundValue);

	return levelsetsGrid;
}


// return openvdb levelsets
VDBLevelsetsPtr PolyLevelsets::getVDBLevelsets()
{
	VDBLevelsetsGrid = VDBcreateLevelsets();
	return VDBLevelsetsGrid;
}
