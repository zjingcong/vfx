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

        FloatGrid::Ptr getNoiseGrid()   {return myGrid;}
        BBox getNoiseBBox() {return noiseBBox;}

    private:
        Noise& noise;
        float voxelSize;

        FloatGrid::Ptr myGrid;  // gird used for stamping noise
        Transform::Ptr xform;   // get the transform of noise grid
        BBox noiseBBox;

        void createNoiseGrid();
};


NoiseGrid::NoiseGrid(Noise& n, float s): noise(n), voxelSize(s)
{
    // create noise grid with specified voxel size
    myGrid = FloatGrid::create();
    xform = myGrid -> transformPtr();
    xform = Transform::createLinearTransform(voxelSize);
    myGrid -> setTransform(xform);

    createNoiseGrid();
}


void NoiseGrid::createNoiseGrid()
{
    // get noise parameters
    Noise_t myPerlinParam = noise.getNoiseParameters();
    float pscale = myPerlinParam.pscale;
    float gamma = myPerlinParam.gamma;  // fade power
    float roughness = myPerlinParam.roughness;
    float octaves = myPerlinParam.octaves;
    Vector P = myPerlinParam.P;

    // get noise bounding box
    Vec3s xyz(P.X(), P.Y(), P.Z());
    Coord ijk = xform -> worldToIndexNodeCentered(xyz);
    int radius_n = std::floor(pscale / voxelSize) + 1;
    Coord ijk0(ijk.x() - radius_n, ijk.y() - radius_n, ijk.z() - radius_n);
    Coord ijk1(ijk.x() + radius_n, ijk.y() + radius_n, ijk.z() + radius_n);
    Vec3s pos_min = xform -> indexToWorld(ijk0);
    Vec3s pos_max = xform -> indexToWorld(ijk1);
    noiseBBox = BBox(pos_min, pos_max);

    float scale = pow( 1.0 + roughness, octaves - 1.0);
    // float scale = (1 - roughness) / float(1 - pow(roughness, octaves));
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
                    // float value = (float(noise.eval(myxyz)) * scale / (2)) + 0.5;

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
}

# endif
