
# include <stdlib.h>
# include <cmath>

# include "Wisp.h"

using namespace lux;


SingleGuideWisp::SingleGuideWisp(Noise& n, WispParms& w, float s):
        guideParticle(n), wisp_parms(w), voxelSize(s), dot_num(w.dot_num)
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
    int i = ijk.x();    int j = ijk.y();    int k = ijk.z();
    FloatGrid::Accessor accessor = myGrid -> getAccessor();

    float opacity = wisp_parms.opacity;
    float value;
    Coord iijjkk;
    float w = voxelSize;
    float weight0 = (1 - w) * (1 - w) * (1 - w);
    float weight1 = w * (1 - w) * (1 - w);
    float weight2 = w * w * (1 - w);
    float weight3 = w * w * w;
    // # pragma omp critical
    // {
    // i, j, k
    value = accessor.getValue(ijk);
    accessor.setValue(ijk, value + opacity * weight0);  // (1 - w) * (1 - w) * (1 - w)
    // i+1, j, k
    iijjkk = Coord(i + 1, j, k);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight1); // w * (1 - w) * (1 - w)
    // i, j+1, k
    iijjkk = Coord(i, j + 1, k);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight1); // (1 - w) * w * (1 - w)
    // i, j, k+1
    iijjkk = Coord(i, j, k + 1);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight1); // (1 - w) * (1 - w) * w
    // i+1, j+1, k
    iijjkk = Coord(i + 1, j + 1, k);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight2);   // w * w * (1 - w)
    // i+1, j, k+1
    iijjkk = Coord(i + 1, j, k + 1);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight2);   // w * (1 - w) * w
    // i, j+1, k+1
    iijjkk = Coord(i, j + 1, k + 1);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight2);   // (1 - w) * w * w
    // i+1, j+1, k+1
    iijjkk = Coord(i + 1, j + 1, k + 1);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight3);   // w * w * w
    // }
}


void SingleGuideWisp::createDot()
{
    // create a point
    double x = 2 * drand48() - 1;
    double y = 2 * drand48() - 1;
    double z = 2 * drand48() - 1;
    // move to surface of a sphere
    Vector xyz(x, y, z);
    double radius = xyz.magnitude();
    double xs = x / radius;
    double ys = y / radius;
    double zs = z / radius;
    // move off the sphere
    Noise_t FSPNParms = guideParticle.getNoiseParameters();
    float clump = wisp_parms.clump;
    double FSPN1scale = pow( 1.0 + FSPNParms.roughness, FSPNParms.octaves - 1.0);
    double FSPNValue = (guideParticle.eval(xyz) * 2) / FSPN1scale;
    // double FSPNValue = guideParticle.eval(xyz);
    double R = pow(fabs(FSPNValue), clump);
    xs *= R;
    ys *= R;
    zs *= R;
    // transform to world space
    Vector xyzs(xs, ys, zs);
    Vector P = FSPNParms.P;
    float pscale = FSPNParms.pscale;
    Vector xw = xyzs * pscale + P;
    // displace
    float delta_x = wisp_parms.delta_x;
    Noise* FSPN2 = wisp_parms.FSPN2;
    Noise_t FSPN2Parms = FSPN2->getNoiseParameters();
    double FSPN2scale = pow(1.0 + FSPN2Parms.roughness, FSPN2Parms.octaves - 1.0);
    Vector xyzs1(xs + delta_x, ys + delta_x, zs + delta_x);
    Vector xyzs2(xs - delta_x, ys - delta_x, zs - delta_x);
    double dx = (FSPN2 -> eval(xyzs)) * 2 / FSPN2scale;
    double dy = (FSPN2 -> eval(xyzs1)) * 2 / FSPN2scale;
    double dz = (FSPN2 -> eval(xyzs2)) * 2 / FSPN2scale;
    // double dx = FSPN2 -> eval(xyzs);
    // double dy = FSPN2 -> eval(xyzs1);
    // double dz = FSPN2 -> eval(xyzs2);
    float dscale = wisp_parms.dscale;
    Vector dxyz(dx, dy, dz);
    Vector D = dxyz * dscale;
    xw += D;
    // stamp to grid
    burnDot(xw);
}


void SingleGuideWisp::createWispDots()
{
    // # pragma omp parallel for
    for (int i = 0; i < dot_num; i++)
    {
        createDot();
    }

    // get wisp bbox
    int min_i = 1000000;    int max_i = -1000000;
    int min_j = 1000000;    int max_j = -1000000;
    int min_k = 1000000;    int max_k = -1000000;
    for (FloatGrid::ValueOnIter iter = myGrid -> beginValueOn(); iter; ++iter)
    {
        Coord ijk = iter.getCoord();
        int i = ijk.x();    int j = ijk.y();    int k = ijk.z();
        if (i < min_i)	{min_i = i;}	if (i > max_i)	{max_i = i;}
        if (j < min_j)	{min_j = j;}	if (j > max_j)	{max_j = j;}
        if (k < min_k)	{min_k = k;}	if (k > max_k)	{max_k = k;}
    }
    Coord min(min_i, min_j, min_k);
    Coord max(max_i, max_j, max_k);
    Vec3s min_pos = xform -> indexToWorld(min);
    Vec3s max_pos = xform -> indexToWorld(max);
    wispBBox = BBox(min_pos, max_pos);
}
