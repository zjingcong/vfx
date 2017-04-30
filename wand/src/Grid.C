
# include "Grid.h"

using namespace lux;


// ---------------------------- Class FloatGridVolume -------------------------------------------

const float VDBLevelsetsVolume::eval(const Vector& x) const
{
    Vec3s xyz(float(x.X()), float(x.Y()), float(x.Z()));	// world space
    // construct a float grid box sampler to perform trilinear interpolation
    openvdb::tools::GridSampler<FloatGrid, openvdb::tools::BoxSampler> sampler(*myFloatGrid);
    float gridValue;
    // openvdb use negative values as levelsets inside and positive values as levelsets outside
    gridValue = -sampler.wsSample(xyz);	// world space sample

	return gridValue;
}

// ----------------------------------------------------------------------------------------------

// ---------------------------- Class FloatGridVolume -------------------------------------------

FloatGridVolume::FloatGridVolume(FloatGrid::Ptr g): grid(g) {}


const float FloatGridVolume::eval(const Vector& x) const
{
	Vec3s xyz(float(x.X()), float(x.Y()), float(x.Z()));	// world space
	// construct a float grid box sampler to perform trilinear interpolation
	openvdb::tools::GridSampler<FloatGrid, openvdb::tools::BoxSampler> sampler(*grid);
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


// ---------------------------- Class VectorGridVolume -------------------------------------------

const Vector VectorGridVolume::eval(const Vector& x) const
{
	Vec3s xyz(float(x.X()), float(x.Y()), float(x.Z()));	// world space
	// construct a color grid box sampler to perform trilinear interpolation
	openvdb::tools::GridSampler<Vec3fGrid, openvdb::tools::BoxSampler> sampler(*vectorGrid);
	Vec3s gridValue;
	gridValue = sampler.wsSample(xyz);	// world space sample
	Vector vecValue(gridValue.x(), gridValue.y(), gridValue.z());

	return vecValue;
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
	// create the color grid
	myGrid = Vec4fGrid::create();
	// get the grid transform
	transform = myGrid -> transformPtr();
	// set voxel size
	transform = Transform::createLinearTransform(voxelSize);
	myGrid -> setTransform(transform);

	createVolumeGrid();
}


ColorVolumeToGrid::ColorVolumeToGrid(Volume<Color>& f, FloatGrid::Ptr floatGrid):
		myVolume(f), floatGrid(floatGrid)
{
	Vec3s voxel = floatGrid->voxelSize();
	voxelSize = voxel.x();
	// create the color grid
	myGrid = Vec4fGrid::create();
	// get the grid transform
	transform = myGrid -> transformPtr();
	// set voxel size
	transform = Transform::createLinearTransform(voxelSize);
	myGrid -> setTransform(transform);

	maskVolumeGrid();
}


ColorVolumeToGrid::ColorVolumeToGrid(Volume<Color>& f, FloatGrid::Ptr floatGrid, Vec4fGrid::Ptr c):
		myVolume(f), floatGrid(floatGrid), myGrid(c)
{
	Vec3s voxel = c->voxelSize();
	voxelSize = voxel.x();
	transform = myGrid -> transformPtr();

	maskVolumeGrid();
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


void ColorVolumeToGrid::maskVolumeGrid()
{
	double start_time = omp_get_wtime();
	double exe_time = omp_get_wtime() - start_time;
	Vec4fGrid::Accessor accessor = myGrid -> getAccessor();
	// # pragma omp parallel for
	for (FloatGrid::ValueOnIter iter = floatGrid->beginValueOn(); iter; ++iter )
	{
		Coord ijk = iter.getCoord();
		if (*iter > 0)
		{
			Vec4s pre_color = accessor.getValue(ijk);
			Vec3s gridPointPos = transform -> indexToWorld(ijk);
			lux::Vector vec(gridPointPos.x(), gridPointPos.y(), gridPointPos.z());
			Color value = myVolume.eval(vec);
			Vec4s colorValue;
			if (pre_color > Vec4s(0.0, 0.0, 0.0, 0.0))
				{colorValue = (Vec4s(value.X(), value.Y(), value.Z(), value.W()) + pre_color); }
			else
				{colorValue = Vec4s(value.X(), value.Y(), value.Z(), value.W());}
			// # pragma omp critical
			{
				accessor.setValue(ijk, colorValue);
			}
		}
	}
	std::cout << "	 | Elapsed Time: " << exe_time << "s" << std::endl;
}


Vec4fGrid::Ptr ColorVolumeToGrid::getVolumeGrid()
{
	return myGrid;
}


// ---------------------------- Class VectorVolumeToGrid ----------------------------------------

VectorVolumeToGrid::VectorVolumeToGrid(Volume<Vector> &f, float s, BBox &bbox):
		vectorVolume(f), voxelSize(s), volumeBBox(bbox)
{
	// create the vector grid
	grid = Vec3fGrid::create();
	// get the grid transform
	transform = grid -> transformPtr();
	// set voxel size
	transform = Transform::createLinearTransform(voxelSize);
	grid -> setTransform(transform);
}


void VectorVolumeToGrid::createVolumeGrid()
{
	double start_time = omp_get_wtime();
	Vec3fGrid::Accessor accessor = grid -> getAccessor();
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
				Vector value = vectorVolume.eval(vec);
				Vec3s valueVec3s(float(value.X()), float(value.Y()), float(value.Z()));
				# pragma omp critical
				{
					accessor.setValue(ijk, valueVec3s);
				}
			}
		}
	}
	double exe_time = omp_get_wtime() - start_time;
	std::cout << "	 | Elapsed Time: " << exe_time << "s" << std::endl;
}


Vec3fGrid::Ptr VectorVolumeToGrid::getVolumeGrid()
{
	createVolumeGrid();
	return grid;
}


BBox VectorVolumeToGrid::getBBox()
{
	BBox gridBBox = getGridBBox<Vec3fTree>(grid);

	return gridBBox;
}

// -----------------------------------------------------------------------------------------
