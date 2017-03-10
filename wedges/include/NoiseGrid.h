# ifndef __NOISEGRID_H__
# define __NOISEGRID_H__

# include <iostream>
# include "math.h"

# include "Noise.h"
# include "PerlinNoise.h"
# include "Types.h"
# include "Vector.h"

using namespace lux;

class NoiseGrid
{
    public:
        NoiseGrid(Noise& n, float s);
        ~NoiseGrid()    {}

        FloatGrid::Ptr getNoiseGrid();

    private:
        Noise& noise;
        float voxelSize;

        FloatGrid::Ptr myGrid;  // gird used for stamping noise
        Transform::Ptr xform;   // get the transform of noise grid
};


NoiseGrid::NoiseGrid(Noise& n, float s): noise(n), voxelSize(s)
{
    // create noise grid with specified voxel size
    myGrid = FloatGrid::create();
    xform = myGrid -> transformPtr();
    xform = Transform::createLinearTransform(voxelSize);
    myGrid -> setTransform(xform);
}


FloatGrid::Ptr NoiseGrid::getNoiseGrid()
{
    // get noise parameters
    Noise_t myPerlinParam = noise.getNoiseParameters();
    float pscale = myPerlinParam.pscale;

    float gamma = myPerlinParam.gamma;  // fade power
    float roughness = myPerlinParam.roughness;
    float octaves = myPerlinParam.octaves;
    Vector P = myPerlinParam.P;
    Vec3s xyz(P.X(), P.Y(), P.Z());
    Coord ijk = xform -> worldToIndexNodeCentered(xyz);
    int radius_n = std::floor(pscale / voxelSize) + 1;
    float scale = pow( 1.0 + roughness, octaves - 1.0);

    std::cout << "Stamping noise..." << std::endl;
    // get noise grid's accessor
    FloatGrid::Accessor accessor = myGrid -> getAccessor();
    // loop over grid point
    for (int i = ijk.x() - radius_n; i <= ijk.x() + radius_n; ++i)
    {
        for (int j = ijk.y() - radius_n; j <= ijk.y() + radius_n; ++j)
        {
            # pragma omp parallel for
            for (int k = ijk.z() - radius_n; k <= ijk.z() + radius_n; ++k)
            {
                // calculate the fade factor
                Coord myijk(i, j, k);
                Vec3s xyzvec = xform -> indexToWorld(myijk);
                Vector myxyz(xyzvec.x(), xyzvec.y(), xyzvec.z());
                float fadefactor = pow((1 - float((P - myxyz).magnitude()) / pscale), gamma);
                if (fadefactor > 0)
                {
                    float value = (noise.eval(myxyz) + 0.5 * scale) / scale;

                    # pragma omp critical
                    {
                        // stamp the value to the grid
                        accessor.setValue(myijk, value * fadefactor);
                        accessor.setValueOn(myijk);
                    }
                }
            }
        }
    }

    return myGrid;
}

# endif
