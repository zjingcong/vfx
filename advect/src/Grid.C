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

FloatGridVolume::FloatGridVolume(FloatGrid::Ptr g): grid(g)
{
    Vec3s voxel = grid->voxelSize();
    float voxeltmp = getMin(voxel.x(), voxel.y());
    voxelSize = getMin(voxel.z(), voxeltmp);
}


const float FloatGridVolume::eval(const Vector& x) const
{
	Vec3s xyz(x.X(), x.Y(), x.Z());	// world space
	// construct a float grid box sampler to perform trilinear interpolation
	openvdb::tools::GridSampler<FloatGrid, openvdb::tools::BoxSampler> sampler(*grid);
	float gridValue;
	gridValue = sampler.wsSample(xyz);	// world space sample

	return gridValue;
}


const Vector FloatGridVolume::grad(const Vector& x) const
{
    Transform::Ptr transform = grid->transformPtr();
    FloatGrid::Accessor accessor = grid->getAccessor();

    Vec3s pos(float(x.X()), float(x.Y()), float(x.Z()));
    Coord ijk = transform->worldToIndexNodeCentered(pos);
    int i = ijk.x();
    int j = ijk.y();
    int k = ijk.z();

    double grad_x = (accessor.getValue(Coord(i + 1, j, k)) - accessor.getValue(Coord(i - 1, j, k))) / (2.0 * voxelSize);
    double grad_y = (accessor.getValue(Coord(i, j + 1, k)) - accessor.getValue(Coord(i, j - 1, k))) / (2.0 * voxelSize);
    double grad_z = (accessor.getValue(Coord(i, j, k + 1)) - accessor.getValue(Coord(i, j, k - 1))) / (2.0 * voxelSize);

    Vector grad(grad_x, grad_y, grad_z);

	return grad;
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

// tag = 0: density stamping
// tag = 1: light stamping
FloatVolumeToGrid::FloatVolumeToGrid(Volume<float>& f, float s, BBox& bbox, int tag):
	myVolume(f), voxelSize(s), volumeBBox(bbox), tag(tag) {}


void FloatVolumeToGrid::createVolumeGrid(float bg)
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

				bool stamping = false;
                // lighting stamp
				if (tag == LIGHT_STAMP)
				{
					if (value <= bg)
					{
						for (int ii = i - 1; ii <= i + 1; ++ii)
						{
							for (int jj = j - 1; jj <= j + 1; ++jj)
							{
								for (int kk = k - 1; kk <= k + 1; ++kk)
								{
                                    Coord iijjkk(ii, jj, kk);   // get neighbors coord
									Vec3s neiPos = transform->indexToWorld(iijjkk);
									lux::Vector neiVec(neiPos.x(), neiPos.y(), neiPos.z());
									float neiValue = myVolume.eval(neiVec); // get neighbors value
									if (neiValue > bg)
									{
										stamping = true;
										goto endLoop;	// end for loop
									}
								}
							}
						}
						endLoop: ;
					}
					else {stamping = true;}
				}
				// density stamp
                else if (tag == DENSITY_STAMP)
				{
					if (value > bg)	{stamping = true;}	// only stamp positive value
				}

				if (stamping)
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


FloatGrid::Ptr FloatVolumeToGrid::getVolumeGrid(float bg)
{
    // create the float grid
    myGrid = FloatGrid::create(bg);
    // get the grid transform
    transform = myGrid -> transformPtr();
    // set voxel size
    transform = Transform::createLinearTransform(voxelSize);
    myGrid -> setTransform(transform);

    createVolumeGrid(bg);

    return myGrid;
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
	double start_time = omp_get_wtime();
	Vec4fGrid::Accessor accessor = myGrid -> getAccessor();
	Vec3s llc = volumeBBox.min();
	Vec3s urc = volumeBBox.max();
	Coord ijk0 = transform -> worldToIndexNodeCentered(llc);
	Coord ijk1 = transform -> worldToIndexNodeCentered(urc);
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
				Color value = myVolume.eval(vec);
				Vec4s mycolor(value.X(), value.Y(), value.Z(), value.W());

				# pragma omp critical
				{
					accessor.setValue(ijk, mycolor);
				}
			}
		}
	}
	double exe_time = omp_get_wtime() - start_time;
	std::cout << "	 | Elapsed Time: " << exe_time << "s" << std::endl;
}


Vec4fGrid::Ptr ColorVolumeToGrid::getVolumeGrid()
{
	createVolumeGrid();
	return myGrid;
}

// ----------------------------------------------------------------------------------------------
