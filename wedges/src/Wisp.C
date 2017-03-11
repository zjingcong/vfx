
# include <stdlib.h>
# include <cmath>

# include "Wisp.h"

using namespace lux;


SingleGuideWisp::SingleGuideWisp(Noise& n, WispParms& w, float s, int num):
        guideParticle(n), wisp_parms(w), voxelSize(s), dot_num(num)
{
    // create noise grid with specified voxel size
    myGrid = FloatGrid::create(0.0);
    xform = myGrid -> transformPtr();
    xform = Transform::createLinearTransform(voxelSize);
    myGrid -> setTransform(xform);

    createWispDots();
}


void SingleGuideWisp::burnDot(Vector xw)
{
    Vec3s xw_pos(xw.X(), xw.Y(), xw.Z());
    Coord ijk = xform -> worldToIndexNodeCentered(xw_pos);
    FloatGrid::Accessor accessor = myGrid -> getAccessor();

    // test here
    float opacity = wisp_parms.opacity;
    accessor.setValue(ijk, opacity);
}


void SingleGuideWisp::createDot()
{
    // create a point
    double x = 2 * drand48() - 1;
    double y = 2 * drand48() - 1;
    double z = 2 * drand48() - 1;
    // move to surface of a sphere
    double radius = pow((x * x, y * y, z * z), 0.5);
    double xs = x / radius;
    double ys = y / radius;
    double zs = z / radius;
    // move off the sphere
    Vector xyz(x, y, z);
    float clump = wisp_parms.clump;
    float FSPNValue = guideParticle.eval(xyz);
    double R = pow(fabs(FSPNValue), clump);
    xs *= R;
    ys *= R;
    zs *= R;
    // transform to world space
    Vector xyzs(xs, ys, zs);
    Noise_t FSPNParms = guideParticle.getNoiseParameters();
    Vector P = FSPNParms.P;
    float pscale = FSPNParms.pscale;
    Vector xw = xyzs * pscale + P;
    // displace
    float delta_x = wisp_parms.delta_x;
    Noise* FSPN2 = wisp_parms.FSPN2;
    Vector xyzs1(xs + delta_x, ys + delta_x, zs + delta_x);
    Vector xyzs2(xs - delta_x, ys - delta_x, zs - delta_x);
    float dx = FSPN2 -> eval(xyzs);
    float dy = FSPN2 -> eval(xyzs1);
    float dz = FSPN2 -> eval(xyzs2);
    float dscale = wisp_parms.dscale;
    Vector dxyz(dx, dy, dz);
    Vector D = dxyz * dscale;
    xw += D;
    // stamp to grid
    burnDot(xw);
}


void SingleGuideWisp::createWispDots()
{
    for (int i = 0; i < dot_num; i++)
    {
        createDot();
    }
}


FloatGrid::Ptr SingleGuideWisp::getWispGrid()
{
    return myGrid;
}
