
# include <stdlib.h>
# include <cmath>
# include "omp.h"

# include "Wisp.h"

using namespace lux;


// create a new grid
SingleGuideWisp::SingleGuideWisp(WispParms& w, float s):
        wisp_parms(w), guideParticle(w.FSPN1), FSPN2(w.FSPN2), dot_num(w.dot_num)
{
    voxelSize = Vec3s(s, s, s);
    // get parms
    FSPN1Parms = guideParticle->getNoiseParameters();
    FSPN2Parms = FSPN2->getNoiseParameters();
    // create noise grid with specified voxel size
    wispGrid = FloatGrid::create(0.0);
    xform = wispGrid -> transformPtr();
    xform = Transform::createLinearTransform(s);
    wispGrid -> setTransform(xform);

    createWispDots();
}


// burn dot on a given grid
SingleGuideWisp::SingleGuideWisp(WispParms& w, FloatGrid::Ptr g):
        wisp_parms(w), guideParticle(w.FSPN1), FSPN2(w.FSPN2), dot_num(w.dot_num), wispGrid(g)
{
    voxelSize = wispGrid->voxelSize();
    // get parms
    FSPN1Parms = guideParticle->getNoiseParameters();
    FSPN2Parms = FSPN2->getNoiseParameters();
    xform = wispGrid->transformPtr();

    createWispDots();
}


void SingleGuideWisp::burnDot(Vector xw)
{
    Vec3s xw_pos(float(xw.X()), float(xw.Y()), float(xw.Z()));
    Coord ijk = xform -> worldToIndexNodeCentered(xw_pos);
    int i = ijk.x();    int j = ijk.y();    int k = ijk.z();
    FloatGrid::Accessor accessor = wispGrid -> getAccessor();

    float opacity = wisp_parms.opacity;
    float value;
    Coord iijjkk;
    float wx = voxelSize.x();
    float wy = voxelSize.y();
    float wz = voxelSize.z();
    float weight0 = (1 - wx) * (1 - wy) * (1 - wz);
    float weight1 = wx * (1 - wy) * (1 - wz);
    float weight2 = (1 - wx) * wy * (1 - wz);
    float weight3 = (1 - wx) * (1 - wy) * wz;
    float weight4 = wx * wy * (1 - wz);
    float weight5 = wx * (1 - wy) * wz;
    float weight6 = (1 - wx) * wy * wz;
    float weight7 = wx * wy * wz;
    // # pragma omp critical
    // {
    // i, j, k
    value = accessor.getValue(ijk);
    accessor.setValue(ijk, value + opacity * weight0);  // (1 - wx) * (1 - wy) * (1 - wz)
    // i+1, j, k
    iijjkk = Coord(i + 1, j, k);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight1); // wx * (1 - wy) * (1 - wz)
    // i, j+1, k
    iijjkk = Coord(i, j + 1, k);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight2); // (1 - wx) * wy * (1 - wz)
    // i, j, k+1
    iijjkk = Coord(i, j, k + 1);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight3); // (1 - wx) * (1 - wy) * wz
    // i+1, j+1, k
    iijjkk = Coord(i + 1, j + 1, k);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight4);   // wx * wy * (1 - wz)
    // i+1, j, k+1
    iijjkk = Coord(i + 1, j, k + 1);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight5);   // wx * (1 - wy) * wz
    // i, j+1, k+1
    iijjkk = Coord(i, j + 1, k + 1);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight6);   // (1 - wx) * wy * wz
    // i+1, j+1, k+1
    iijjkk = Coord(i + 1, j + 1, k + 1);
    value = accessor.getValue(iijjkk);
    accessor.setValue(iijjkk, value + opacity * weight7);   // wx * wy * wz
    // }
}


void SingleGuideWisp::createDot()
{
    // create a point
    double x = 2 * drand48() - 1;
    double y = 2 * drand48() - 1;
    double z = 2 * drand48() - 1;
    // create offset
    Vector offset = wisp_parms.offset;
    x += offset.X();
    y += offset.Y();
    z += offset.Z();
    // move to surface of a sphere
    Vector xyz(x, y, z);
    double radius = xyz.magnitude();
    double xs = x / radius;
    double ys = y / radius;
    double zs = z / radius;
    // move off the sphere
    float clump = wisp_parms.clump;
    double FSPN1scale = pow( 1.0 + FSPN1Parms.roughness, FSPN1Parms.octaves - 1.0);
    double FSPNValue = (guideParticle->eval(xyz) * 2) / FSPN1scale;
    // double FSPNValue = guideParticle.eval(xyz);
    double R = pow(fabs(FSPNValue), clump);
    xs *= R;
    ys *= R;
    zs *= R;
    // transform to world space
    Vector xyzs(xs, ys, zs);
    Vector P = FSPN1Parms.P;
    float pscale = FSPN1Parms.pscale;
    Vector xw = xyzs * pscale + P;
    // displace
    float delta_x = wisp_parms.delta_x;
    double FSPN2scale = pow(1.0 + FSPN2Parms.roughness, FSPN2Parms.octaves - 1.0);
    Vector xyzs1(xs + delta_x, ys + delta_x, zs + delta_x);
    Vector xyzs2(xs - delta_x, ys - delta_x, zs - delta_x);
    double dx = (FSPN2 -> eval(xyzs)) * 2 / FSPN2scale;
    double dy = (FSPN2 -> eval(xyzs1)) * 2 / FSPN2scale;
    double dz = (FSPN2 -> eval(xyzs2)) * 2 / FSPN2scale;
    float dscale = wisp_parms.dscale * float(pow(xyzs.magnitude() * pscale, wisp_parms.exp));
    Vector dxyz(dx, dy, dz);
    Vector D = dxyz * dscale;
    xw += D;
    // stamp to grid
    burnDot(xw);
}


void SingleGuideWisp::createWispDots()
{
    double start_time = omp_get_wtime();
    // # pragma omp parallel for
    for (int i = 0; i < dot_num; i++)
    {
            createDot();
    }
    double exe_time = omp_get_wtime() - start_time;
    std::cout << "	 | Elapsed Time: " << exe_time << "s" << std::endl;
    // get wisp bbox
    int min_i = 1000000;    int max_i = -1000000;
    int min_j = 1000000;    int max_j = -1000000;
    int min_k = 1000000;    int max_k = -1000000;
    for (FloatGrid::ValueOnIter iter = wispGrid -> beginValueOn(); iter; ++iter)
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
