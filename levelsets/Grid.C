# include <openvdb/tools/Interpolation.h>

# include "Grid.h"

using namespace lux;

const float FloatGridVolume::eval(const Vector& x) const
{
	Vec3s xyz(x.X(), x.Y(), x.Z());	// world space
	// construct a float grid box sampler to perform trilinear interpolation
	openvdb::tools::GridSampler<FloatGrid, openvdb::tools::BoxSampler> sampler(floatGrid);
	float gridValue;
	gridValue = sampler.wsSample(xyz);	// world space sample

	return gridValue;
}

