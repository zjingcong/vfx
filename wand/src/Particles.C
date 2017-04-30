# include "Particles.h"

WispCloud:: WispCloud(float i, Vector sp, Vector ep, Vector dp, float dpf, int l, int seed):
        interval(i), start_pos(sp), end_pos(ep), dir(dp), dp_factor(dpf), life_time(l), seed(seed)
{
    initialize();
}


void WispCloud::initialize()
{
    // generate particle map

    Particle p0;
    p0.id = 0;
    p0.age = 0;
    p0.lifetime = life_time;
    p0.isActive = true;
    p0.pos = start_pos;
    cloud[0] = p0;
    int i = 1;
    while (true)
    {
        Particle p;
        p.id = i;
        p.age = -i;
        p.lifetime = life_time;
        p.isActive = true;
        // generate position and tangent
        Vector hemisphereRandVec = randm::hemisphereRand(i + seed);
        double theta_degree = acos(hemisphereRandVec * dir) * 180.0 / M_PI;
        Vector axis = dir ^ hemisphereRandVec;
        Vector tangentRand = vecRotation(hemisphereRandVec, axis, float(theta_degree));
        Vector tangent = (1 - dp_factor) * tangentRand + dp_factor * dir;
        tangent = tangent.unitvector();
        p.tangent = tangent;

        Vector pos = cloud[i-1].pos + (tangent * interval);
        double distance = (pos - end_pos).magnitude();
        p.pos = pos;
        cloud[i] = p;
        i++;

        if (distance <= interval)   {goto endwhileloop;}
    }
    endwhileloop: ;
    particle_num = i;
    Particle pe;
    pe.id = i;
    pe.age = -i;
    pe.lifetime = life_time;
    pe.isActive = true;
    pe.pos = end_pos;
    pe.tangent = (end_pos - cloud[i-1].pos).unitvector();
    cloud[i] = pe;

    cout << "\t | particle num: " << particle_num << endl;

    // generate wisp map
    for (int j = 0; j < particle_num; ++j)
    {
        WispParms* wispParameters = new WispParms;
        FractalSum<PerlinNoiseGustavson>* FSPN1 = new FractalSum<PerlinNoiseGustavson>;
        FractalSum<PerlinNoiseGustavson>* FSPN2 = new FractalSum<PerlinNoiseGustavson>;
        wispParameters->FSPN1 = FSPN1;
        wispParameters->FSPN2 = FSPN2;
        Noise_t FSPN1Parms;
        Noise_t FSPN2Parms;
        FSPN1Parms.translate = cloud[j].pos;
        FSPN2Parms.translate = cloud[j].pos;
        wispParameters->opacity = 0.006f;
        wispParameters->FSPN1->setParameters(FSPN1Parms);
        wispParameters->FSPN2->setParameters(FSPN2Parms);
        wispParmsMap[j] = wispParameters;
    }
}


void WispCloud::spendTime(int delta_time, int update_type)
{
    // update particle status
    for (int i = 0; i < particle_num; ++i)
    {
        if (cloud[i].isActive)
        {
            cloud[i].age += delta_time;
            if (cloud[i].age > cloud[i].lifetime)  {cloud[i].isActive = false;}    // dead
        }
    }
    // update wisp parms
    for (int i = 0; i < particle_num; ++i)
    {
        if (cloud[i].isActive)
        {
            switch (update_type)
            {
                case 0:
                {
                    WispParms* wispParameters = wispParmsMap[i];
                    float young = (cloud[i].lifetime - cloud[i].age) / float(cloud[i].lifetime);
                    float old = cloud[i].age / float(cloud[i].lifetime);
                    Noise_t FSPN1Parms = wispParameters->FSPN1->getNoiseParameters();
                    Noise_t FSPN2Parms = wispParameters->FSPN2->getNoiseParameters();
                    // FSPN1
                    FSPN1Parms.P = cloud[i].pos;
                    // FSPN1Parms.frequency = 0.5531;
                    // FSPN1Parms.octaves = 1.5;
                    FSPN1Parms.pscale = 1.5f;
                    FSPN1Parms.translate += delta_time * cloud[i].tangent * interval;
                    // FSPN2
                    FSPN2Parms.octaves = 2.2;
                    FSPN2Parms.frequency = 2.5343;
                    FSPN2Parms.fjump = 2.5;
                    FSPN2Parms.translate += delta_time * cloud[i].tangent * interval * 0.5;
                    // update parms
                    wispParameters->FSPN1->setParameters(FSPN1Parms);
                    wispParameters->FSPN2->setParameters(FSPN2Parms);

                    wispParameters->delta_x = 1.5;
                    wispParameters->dscale = 1.0f * pow(old, 2.0f) + 0.3f;
                    if (wispParameters->opacity < 0.0005f)
                        { wispParameters->opacity = 0.0005f; }
                    else    {wispParameters->opacity *= pow(young, 2.0f);}
                    wispParameters->dot_num = 300000;
                    wispParameters->clump = 1.5f;
                    wispParameters->exp = 1.2f;
                    wispParameters->offset = -cloud[i].tangent * 3.0;

                    break;
                }

//                case 1:
//                {
//                    WispParms* wispParameters = wispParmsMap[i];
//                    float young = (cloud[i].lifetime - cloud[i].age) / float(cloud[i].lifetime);
//                    float old = cloud[i].age / float(cloud[i].lifetime);
//                    Noise_t FSPN1Parms = wispParameters->FSPN1->getNoiseParameters();
//                    Noise_t FSPN2Parms = wispParameters->FSPN2->getNoiseParameters();
//                    FSPN1Parms.P = cloud[i].pos + cloud[i].tangent * interval;
//                    FSPN1Parms.translate = cloud[i].pos;
//                    FSPN2Parms.frequency = 4.5343;
//                    FSPN2Parms.fjump = 2.5;
//                    FSPN2Parms.octaves = 2.4;
//                    FSPN2Parms.translate = cloud[i].pos;
//                    wispParameters->dot_num = 500000;
//
//                    wispParameters->clump = 0.233f;
//                    FSPN1Parms.pscale = 1.2f * old;
//
//                    // update parms
//                    wispParameters->FSPN1->setParameters(FSPN1Parms);
//                    wispParameters->FSPN2->setParameters(FSPN2Parms);
//
//                    wispParameters->opacity = 0.005f;
//                    wispParameters->delta_x = 1.5;
//                    wispParameters->dscale = 0.5;
//
//                    break;
//                }

                default:
                    return;
            }
        }
    }
}


void WispCloud::testSys()
{
    for (int i = 0; i < particle_num; ++i)
    {
        if (cloud[i].age > 0 && cloud[i].isActive)
        {
            std::cout << "particle id: " << i << " age: " << cloud[i].age << std::endl;
        }
    }
}


void WispCloud::stampWispCloudGrid(FloatGrid::Ptr wispGrid)
{
    for (int i = 0; i < particle_num; ++i)
    {
        if (cloud[i].age > 0 && cloud[i].isActive)
        {
            std::cout << "Stamping wisp " << i << "..." << std::endl;
            float pscale = wispParmsMap[i]->FSPN1->getNoiseParameters().pscale;
            SingleGuideWisp wisp(*wispParmsMap[i], wispGrid);
        }
    }
}
