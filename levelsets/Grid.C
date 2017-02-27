# include <openvdb/tools/Interpolation.h>
# include <iostream>

# include "Grid.h"

using namespace lux;


const float FloatGridVolume::eval(const Vector& x) const
{
	// FloatGrid::Accessor accessor = myFloatGrid.getAccessor();

	Vec3s xyz(x.X(), x.Y(), x.Z());	// world space
	// std::cout << "pos: " << xyz << std::endl;

	// construct a float grid box sampler to perform trilinear interpolation
	openvdb::tools::GridSampler<FloatGrid, openvdb::tools::BoxSampler> sampler(*myFloatGrid);
	float gridValue;
	gridValue = sampler.wsSample(xyz);	// world space sample

	// std::cout << "sampler: " << sampler.wsSample(xyz) << std::endl;

	Transform::Ptr transform = myFloatGrid -> transformPtr();


	// use negative to test openvdb levelsets(outside is pos and inside is neg)
	return gridValue;
}

