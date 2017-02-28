# include <openvdb/tools/Interpolation.h>
# include <iostream>

# include "Grid.h"

using namespace lux;


// ---------------------------- Class FloatGridVolume -------------------------------------------

const float FloatGridVolume::eval(const Vector& x) const
{
	// FloatGrid::Accessor accessor = myFloatGrid.getAccessor();
	Vec3s xyz(x.X(), x.Y(), x.Z());	// world space
	// construct a float grid box sampler to perform trilinear interpolation
	openvdb::tools::GridSampler<FloatGrid, openvdb::tools::BoxSampler> sampler(*myFloatGrid);
	float gridValue;
	gridValue = sampler.wsSample(xyz);	// world space sample

	// use negative to test openvdb levelsets(outside is pos and inside is neg)
	return gridValue;
}

// ----------------------------------------------------------------------------------------------


// ---------------------------- Class FloatVolumeToGrid -----------------------------------------

FloatVolumeToGrid::FloatVolumeToGrid(Volume<float>& f, float s, BBox& bbox): 
	myVolume(f), voxelSize(s), volumeBBox(bbox)
{
	// create the float grid
	myGrid = FloatGrid::create();
	// get the grid transform
	transform = myGrid -> transformPtr();
	// set voxel size
	transform = Transform::createLinearTransform(voxelSize);
	myGrid -> setTransform(transform);
}

void FloatVolumeToGrid::createVolumeGrid()
{
	FloatGrid::Accessor accessor = myGrid -> getAccessor();
	Vec3s llc = volumeBBox.min();
	Vec3s urc = volumeBBox.max();
	// std::cout << "vec llc: " << llc << std::endl;
	// std::cout << "vec urc: " << urc << std::endl;
	Coord ijk0 = transform -> worldToIndexNodeCentered(llc);
	Coord ijk1 = transform -> worldToIndexNodeCentered(urc);
	// std::cout << "coord llc: " << ijk0 << std::endl;
	// std::cout << "coord urc: " << ijk1 << std::endl;
	for (int i = ijk0.x(); i <= ijk1.x(); ++i)
	{
		for (int j = ijk0.y(); j <= ijk1.y(); ++j)
		{
			for (int k = ijk0.z(); k <= ijk1.z(); ++k)
			{
				Coord ijk(i, j, k);
				Vec3s gridPointPos = transform -> indexToWorld(ijk);
				// std::cout << "vec: " << gridPointPos << std::endl;
				lux::Vector vec(gridPointPos.x(), gridPointPos.y(), gridPointPos.z());
				// std::cout << "Vector: " << vec.X() << " " << vec.Y() << " " << vec.Z() << std::endl;
				float value = myVolume.eval(vec);
				// std::cout << "value: " << value << std::endl;
				accessor.setValue(ijk, value);
			}
		}
	}
}


FloatGrid::Ptr FloatVolumeToGrid::getVolumeGrid()
{
	createVolumeGrid();
	return myGrid;
}

// ----------------------------------------------------------------------------------------------

