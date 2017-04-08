# include <vector>
# include <iostream>
# include <openvdb/tools/MeshToVolume.h>

# include "Levelsets.h"

using namespace lux;


FloatGrid::Ptr PolyLevelsets::VDBcreateLevelsets()
{
	std::vector<Vec3s> points = polyModel.polyPoints;
	std::vector<Vec3I> triangles = polyModel.triIndices;
	Transform::Ptr lXform = Transform::createLinearTransform(voxelSize);
	FloatGrid::Ptr mesh2levelsets = openvdb::tools::meshToLevelSet<FloatGrid>(*lXform, points, triangles, halfWidth);

	// get bounding box
	int min_i = 100000000;	int max_i = -100000000;
	int min_j = 100000000;	int max_j = -100000000;
	int min_k = 100000000;	int max_k = -100000000;
	Transform::Ptr transform = mesh2levelsets->transformPtr();
	for (FloatGrid::ValueOnIter iter = mesh2levelsets->beginValueOn(); iter; ++iter)
	{
		Coord ijk = iter.getCoord();
		int i = ijk.x();
		int j = ijk.y();
		int k = ijk.z();
		if (i < min_i)	{min_i = i;}	if (i > max_i)	{max_i = i;}
		if (j < min_j)	{min_j = j;}	if (j > max_j)	{max_j = j;}
		if (k < min_k)	{min_k = k;}	if (k > max_k)	{max_k = k;}
	}
	Coord min(min_i, min_j, min_k);
	Coord max(max_i, max_j, max_k);
	Vec3s min_pos = transform -> indexToWorld(min);
	Vec3s max_pos = transform -> indexToWorld(max);
	levelsetsBBox = BBox(min_pos, max_pos);
	// get background value
	backgroundValue = mesh2levelsets->background();
	cout << "\t | VDB Levelsets Background: " << backgroundValue << endl;

	return mesh2levelsets;
}


// return the polymodel levelsets
FloatGrid::Ptr PolyLevelsets::getLevelsets()
{
	FloatGrid::Ptr mesh2levelsets = VDBcreateLevelsets();
	cout << "Convert VDB levelsets..." << endl;
	VDBLevelsetsVolume levelsetsVolume(mesh2levelsets);
	FloatVolumeToGrid levelsetsVolume2Grid(levelsetsVolume, voxelSize, levelsetsBBox);
	FloatGrid::Ptr levelsetsGrid = levelsetsVolume2Grid.getVolumeGrid(-backgroundValue);

	return levelsetsGrid;
}


// return openvdb levelsets
VDBLevelsetsPtr PolyLevelsets::getVDBLevelsets()
{
	FloatGrid::Ptr mesh2levelsets = VDBcreateLevelsets();
	return mesh2levelsets;
}
