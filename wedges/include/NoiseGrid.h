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
        NoiseGrid(Noise n, float s): noise(n), voxelSize(s)
            {xform = myGrid -> transformPtr();}
        ~NoiseGrid()    {}

        FloatGrid::Ptr getNoiseGrid();

    private:
        Noise noise;
        float voxelSize;

        FloatGrid::Ptr myGrid;  // gird used for stamping noise
        Transform::Ptr xform;   // get the transform of noise grid
};


FloatGrid::Ptr NoiseGrid::getNoiseGrid()
{
    Noise_t myPerlinParam;
    noise.getParameters(myPerlinParam);
    float pscale = myPerlinParam.pscale;
    std::cout << pscale << std::endl;
    float gamma = myPerlinParam.gamma;  // fade power
    Vector P = myPerlinParam.P;
//    Vec3s xyz(P.X(), P.Y(), P.Z());
//    Coord ijk = xform -> worldToIndexNodeCentered(xyz);
//    int radius_n = std::floor(pscale / voxelSize) + 1;

//    // get noise grid's accessor
//    FloatGrid::Accessor accessor = myGrid -> getAccessor();
//    // loop over grid point
//    for (int i = ijk.x() - radius_n; i <= ijk.x() + radius_n; ++i)
//    {
//        for (int j = ijk.y() - radius_n; j <= ijk.y() + radius_n; ++j)
//        {
//            for (int k = ijk.z() - radius_n; k <= ijk.z() + radius_n; ++k)
//            {
//                // calculate the fade factor
//                Coord myijk(i, j, k);
//                Vec3s xyzvec = xform -> indexToWorld(myijk);
//                Vector myxyz(xyzvec.x(), xyzvec.y(), xyzvec.z());
//                float fadefactor = pow((1 - float((P - myxyz).magnitude()) / pscale), gamma);
//                if (fadefactor > 0)
//                {
//                    float value = noise.eval(myxyz);
//                    // stamp the value to the grid
//                    accessor.setValue(myijk, value * fadefactor);
//                    accessor.setValueOn(myijk);
//                }
//            }
//        }
//    }

    return myGrid;
}

# endif
