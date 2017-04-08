
# include "Pyroclastic.h"

// ----------------------------------- Pyrosphere ------------------------------------

Pyrosphere::Pyrosphere(Noise &n): noise(n)
{
    myPyroParam = noise.getNoiseParameters();
    float r = myPyroParam.pscale;
    Vec3s min(-4 * r, -4 * r, -4 * r);
    Vec3s max(4 * r, 4 * r, 4 * r);
    pyroBBox = BBox(min, max);
}


const float Pyrosphere::eval(const Vector &x) const {
    float value;
    // create the sphere volume
    float radius = myPyroParam.pscale;
    Sphere sphere(radius);
    float gamma = myPyroParam.gamma;
    Vector xyz = x.unitvector() * radius;
    float noiseValue = float(pow(fabs(noise.getAmp() * noise.eval(xyz)), gamma));
    value = sphere.eval(x) + noiseValue;

    return value;
}


// ------------------------------- PyroVDBLevelsets ------------------------------------

PyroVDBLevelsets::PyroVDBLevelsets(VDBLevelsetsPtr l, Noise& n): levelsetsGrid(l), noise(n)
{
    levelsetsVolumePtr = new VDBLevelsetsVolume(levelsetsGrid);
    noiseParm = noise.getNoiseParameters();
}


const float PyroVDBLevelsets::eval(const Vector& x) const
{
    FloatGrid::Accessor acc = levelsetsGrid->getAccessor();
    Transform::Ptr transform = levelsetsGrid->transformPtr();
    Vec3s pos(float(x.X()), float(x.Y()), float(x.Z()));
    Coord ijk = transform->worldToIndexNodeCentered(pos);

    bool doCPT = false;
    if (acc.isValueOn(ijk))
    {
        doCPT = true;
//        int i = ijk.x();
//        int j = ijk.y();
//        int k = ijk.z();
//        std::vector<Coord> coordList;
//        coordList.push_back(ijk);
//        coordList.push_back(Coord(i - 1, j, k));
//        coordList.push_back(Coord(i + 1, j, k));
//        coordList.push_back(Coord(i, j + 1, k));
//        coordList.push_back(Coord(i, j - 1, k));
//        coordList.push_back(Coord(i, j, k + 1));
//        coordList.push_back(Coord(i, j, k - 1));
//        for (Coord iijjkk: coordList)
//        {
//            if (!acc.isValueOn(iijjkk))
//            {
//                doCPT = false;
//                goto endLoop;
//            }
//        }
//        endLoop: ;
    }

    float value;
    float volumeValue = levelsetsVolumePtr->eval(x);
    if (doCPT)
    {
        // using openvdb cpt
        UniformScaleMap::Ptr map = transform->map<UniformScaleMap>();
        // CPT_RANGE will compute in the range space of the map (world space)
        openvdb::math::CPT_RANGE<UniformScaleMap, openvdb::math::CD_2ND> cptROp;
        Vec3s cptResult = cptROp.result(*map, acc, ijk);
        Vector cptVec(cptResult.x(), cptResult.y(), cptResult.z());

        float gamma = noiseParm.gamma;
        float amp = noise.getAmp();
        double scale = pow( 1.0 + noiseParm.roughness, noiseParm.octaves - 1.0);
        // set noise value range to [-amp, amp]
        float val = float(amp * (noise.eval(cptVec) + 0.5 * scale ) / scale);
        float noiseValue = pow(fabs(val), gamma);
        value = levelsetsVolumePtr->eval(x) + noiseValue;
    }
    else    {value = volumeValue;}

    return value;
}
