# include <openvdb/tools/Interpolation.h>

# include "Grid.h"
# include "omp.h"

using namespace lux;


// ---------------------------- Class FloatGridVolume -------------------------------------------

const float VDBLevelsetsVolume::eval(const Vector& x) const
{
	// FloatGrid::Accessor accessor = myFloatGrid.getAccessor();
	Vec3s xyz(x.X(), x.Y(), x.Z());	// world space
	// construct a float grid box sampler to perform trilinear interpolation
	openvdb::tools::GridSampler<FloatGrid, openvdb::tools::BoxSampler> sampler(*myFloatGrid);
	float gridValue;
	// openvdb use negative values as levelsets inside and positive values as levelsets outside
	gridValue = -sampler.wsSample(xyz);	// world space sample

	return gridValue;
}

// ----------------------------------------------------------------------------------------------

// ---------------------------- Class FloatGridVolume -------------------------------------------

const float FloatGridVolume::eval(const Vector& x) const
{
	// FloatGrid::Accessor accessor = myFloatGrid.getAccessor();
	Vec3s xyz(x.X(), x.Y(), x.Z());	// world space
	// construct a float grid box sampler to perform trilinear interpolation
	openvdb::tools::GridSampler<FloatGrid, openvdb::tools::BoxSampler> sampler(*myFloatGrid);
	float gridValue;
	gridValue = sampler.wsSample(xyz);	// world space sample

	return gridValue;
}

// ----------------------------------------------------------------------------------------------


// ---------------------------- Class ColorGridVolume -------------------------------------------

const Color ColorGridVolume::eval(const Vector& x) const
{
	Vec3s xyz(x.X(), x.Y(), x.Z());	// world space
	// construct a color grid box sampler to perform trilinear interpolation
	openvdb::tools::GridSampler<Vec4fGrid, openvdb::tools::BoxSampler> sampler(*myColorGrid);
	Vec4s gridValue;
	gridValue = sampler.wsSample(xyz);	// world space sample
	Color colorValue(gridValue.x(), gridValue.y(), gridValue.z(), gridValue.w());

	return colorValue;
}

// ----------------------------------------------------------------------------------------------

/// =============================================================================================

// ---------------------------- Class FloatVolumeToGrid -----------------------------------------

FloatVolumeToGrid::FloatVolumeToGrid(Volume<float>& f, float s, BBox& bbox): 
	myVolume(f), voxelSize(s), volumeBBox(bbox)
{
	// create the float grid
	myGrid = FloatGrid::create(0.0);
	// get the grid transform
	transform = myGrid -> transformPtr();
	// set voxel size
	transform = Transform::createLinearTransform(voxelSize);
	myGrid -> setTransform(transform);

	createVolumeGrid();
}


void FloatVolumeToGrid::createVolumeGrid()
{
	double start_time = omp_get_wtime();
	FloatGrid::Accessor accessor = myGrid -> getAccessor();
	Vec3s llc = volumeBBox.min();
	Vec3s urc = volumeBBox.max();
	Coord ijk0 = transform -> worldToIndexNodeCentered(llc);
	Coord ijk1 = transform -> worldToIndexNodeCentered(urc);
	int min_i = ijk1.x();	int max_i = ijk0.x();
	int min_j = ijk1.y();	int max_j = ijk0.y();
	int min_k = ijk1.z();	int max_k = ijk0.z();
	# pragma omp parallel for collapse(3)
	for (int i = ijk0.x(); i <= ijk1.x(); ++i)
	{
		for (int j = ijk0.y(); j <= ijk1.y(); ++j)
		{
			for (int k = ijk0.z(); k <= ijk1.z(); ++k)
			{
				Coord ijk(i, j, k);
				Vec3s gridPointPos = transform -> indexToWorld(ijk);
				lux::Vector vec(gridPointPos.x(), gridPointPos.y(), gridPointPos.z());
				float value = myVolume.eval(vec);

				if (value >= 0)	// only stamp positive value
				{
					# pragma omp critical
					{
						if (i < min_i)	{min_i = i;}	if (i > max_i)	{max_i = i;}
						if (j < min_j)	{min_j = j;}	if (j > max_j)	{max_j = j;}
						if (k < min_k)	{min_k = k;}	if (k > max_k)	{max_k = k;}
						accessor.setValue(ijk, value);
					}
				}
			}
		}
	}
	Coord min(min_i, min_j, min_k);
	Coord max(max_i, max_j, max_k);
	Vec3s min_pos = transform -> indexToWorld(min);
	Vec3s max_pos = transform -> indexToWorld(max);
	gridBBox = BBox(min_pos, max_pos);
	double exe_time = omp_get_wtime() - start_time;
	std::cout << "	 | Elapsed Time: " << exe_time << "s" << std::endl;
}

// ----------------------------------------------------------------------------------------------


// ---------------------------- Class ColorVolumeToGrid -----------------------------------------

ColorVolumeToGrid::ColorVolumeToGrid(Volume<Color>& f, float s, BBox& bbox): 
	myVolume(f), voxelSize(s), volumeBBox(bbox)
{
	// create the float grid
	myGrid = Vec4fGrid::create();
	// get the grid transform
	transform = myGrid -> transformPtr();
	// set voxel size
	transform = Transform::createLinearTransform(voxelSize);
	myGrid -> setTransform(transform);
}


void ColorVolumeToGrid::createVolumeGrid()
{
	Vec4fGrid::Accessor accessor = myGrid -> getAccessor();
	Vec3s llc = volumeBBox.min();
	Vec3s urc = volumeBBox.max();
	Coord ijk0 = transform -> worldToIndexNodeCentered(llc);
	Coord ijk1 = transform -> worldToIndexNodeCentered(urc);
	for (int i = ijk0.x(); i <= ijk1.x(); ++i)
	{
		for (int j = ijk0.y(); j <= ijk1.y(); ++j)
		{
			# pragma omp parallel for
			for (int k = ijk0.z(); k <= ijk1.z(); ++k)
			{
				Coord ijk(i, j, k);
				Vec3s gridPointPos = transform -> indexToWorld(ijk);
				lux::Vector vec(gridPointPos.x(), gridPointPos.y(), gridPointPos.z());
				Color value = myVolume.eval(vec);
				Vec4s mycolor(value.X(), value.Y(), value.Z(), value.W());

				# pragma omp critical
				{
					accessor.setValue(ijk, mycolor);
				}
			}
		}
	}
}


Vec4fGrid::Ptr ColorVolumeToGrid::getVolumeGrid()
{
	createVolumeGrid();
	return myGrid;
}

// ----------------------------------------------------------------------------------------------
