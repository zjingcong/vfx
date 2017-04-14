
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

PyroVDBLevelsets::PyroVDBLevelsets(VDBLevelsetsPtr l, Noise& n):
        levelsetsGrid(l),
        noise(n),
        transform(levelsetsGrid->transformPtr()),
        acc(levelsetsGrid->getAccessor()),
        map(transform->map<UniformScaleMap>())
{
    levelsetsVolumePtr = new VDBLevelsetsVolume(levelsetsGrid);
    noiseParm = noise.getNoiseParameters();
}


const float PyroVDBLevelsets::eval(const Vector& x) const
{
    Vec3s pos(float(x.X()), float(x.Y()), float(x.Z()));
    Coord ijk = transform->worldToIndexNodeCentered(pos);

    bool doCPT = false;
    if (acc.isValueOn(ijk))
    {
        doCPT = true;
    }

    float value;
    float volumeValue = levelsetsVolumePtr->eval(x);
    if (doCPT)
    {
        // using openvdb cpt
        Vec3s cptResult = cptROp.result(*map, acc, ijk);
        Vector cptVec(cptResult.x(), cptResult.y(), cptResult.z());

        float gamma = noiseParm.gamma;
        float amp = noise.getAmp();
        double scale = pow( 1.0 + noiseParm.roughness, noiseParm.octaves - 1.0);
        // set noise value range to [-1, 1]
        float val = float((2 * noise.eval(cptVec)) / scale);
        // set (noise value ^ gamma) range to [-amp, amp]
        float noiseValue = amp * pow(fabs(val), gamma);
        value = levelsetsVolumePtr->eval(x) + noiseValue;
    }
    else    {value = volumeValue;}

    return value;
}
