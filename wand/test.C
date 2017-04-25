#include <openvdb/math/FiniteDifference.h>
#include <openvdb/math/Operators.h>
#include <openvdb/tools/LevelSetSphere.h>
#include "Types.h"

int main(int argc, char** argv)
{
    // create a sphere level set and accessor
    FloatGrid::Ptr grid = openvdb::tools::createLevelSetSphere<FloatGrid>(1.0, Vec3f(0,0,0), 0.004);
    FloatGrid::Accessor acc = grid->getAccessor();
    Transform::Ptr transform = grid->transformPtr();
    UniformScaleMap::Ptr map = transform->map<UniformScaleMap>();

    // create a closest point transform operator using a uniform scale map
    // and a 2nd order center difference finite difference scheme
    // CPT_RANGE will compute in the range space of the map (world space)
    openvdb::math::CPT_RANGE<UniformScaleMap, openvdb::math::CD_2ND> cptROp;

    // create a closest point transform operator using a uniform scale map
    // and a 2nd order center difference finite difference scheme
    // CPT will compute in domain space of the map (voxel space)
    openvdb::math::CPT<UniformScaleMap, openvdb::math::CD_2ND> cptOp;

    // Transform a world space coordinate (outside the level set) into index space for CPT
    Vec3f pos(100.5, 0.1, 0.1);
    Coord ijk(transform->worldToIndexNodeCentered(pos));

    // Find the closest point on the surface of level set
    std::cout << pos << " cpt: " << cptOp.result(*map, acc, ijk) << std::endl;
    std::cout << pos << " cptR: " << cptROp.result(*map, acc, ijk) << std::endl;

    return 0;
}
