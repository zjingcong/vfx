
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


const float Pyrosphere::eval(const Vector &x) const
{
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

// --------------------------------- PyroLevelsets ----------------------------------------

PyroLevelsets::PyroLevelsets(FloatGridVolume& e, Noise &n, float bg): levelsetsVolume(e), noise(n), background(bg)
{
    noiseParm = noise.getNoiseParameters();
    levelsetsGrid = levelsetsVolume.getGrid();
    // get CPT
    cptFieldPtr = new LevelsetsCPT(levelsetsVolume);
}


const float PyroLevelsets::eval(const Vector &x) const
{
    float value;
    float levelsetsValue = levelsetsVolume.eval(x);

    bool doCPT = true;
    // outside narrow band
    if (levelsetsValue <= background || levelsetsValue >= -background)   {doCPT = false;}
    // inside narrow band: signed distance function
    else
    {
        // find the neighbors value
        Transform::Ptr transform = levelsetsGrid->transformPtr();
        FloatGrid::Accessor accessor = levelsetsGrid->getAccessor();

        Vec3s pos(float(x.X()), float(x.Y()), float(x.Z()));
        Coord ijk = transform->worldToIndexNodeCentered(pos);
        int i = ijk.x();
        int j = ijk.y();
        int k = ijk.z();
        for (int ii = i - 1; ii <= i + 1; ++ii)
        {
            for (int jj = j - 1; jj <= j + 1; ++jj)
            {
                for (int kk = k - 1; kk <= k + 1; ++kk)
                {
                    Coord iijjkk(ii, jj, kk);
                    float neiValue =  accessor.getValue(iijjkk); // neighbors value
                    if (neiValue <= background || neiValue >= -background)
                    {
                        doCPT = false;
                        goto endLoop;
                    }
                }
            }
        }
        endLoop: ;
    }

    if (doCPT)
    {
        float gamma = noiseParm.gamma;
        Vector Y = cptFieldPtr->eval(x);

        float noiseValue = float(pow(fabs(noise.getAmp() * noise.eval(Y)), gamma));
        value = levelsetsVolume.eval(x) + noiseValue;
    }
    else    {value = levelsetsValue;}

    return value;
}
