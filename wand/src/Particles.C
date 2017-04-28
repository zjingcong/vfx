# include "Particles.h"

WispCloud::WispCloud(int num, float i, Vector sp, Vector dp, float dpf, int seed):
        particle_num(num), interval(i), start_pos(sp), dir(dp), dp_factor(dpf), seed(seed)
{
    initialize();
}


void WispCloud::initialize()
{
    // generate particle map
    for (int i = 0; i < particle_num; ++i)
    {
        Particle p;
        p.id = i;
        p.age = -i;
        p.lifetime = 15;
        p.isActive = true;
        // generate position and tangent
        Vector hemisphereRandVec = randm::hemisphereRand(i + seed);
        double theta_degree = acos(hemisphereRandVec * dir) * 180.0 / M_PI;
        Vector axis = dir ^ hemisphereRandVec;
        Vector tangentRand = vecRotation(hemisphereRandVec, axis, float(theta_degree));
        Vector tangent = ((1 - dp_factor) * tangentRand + dp_factor * Vector(0.0, 0.0, -1.0)).unitvector();
        p.tangent = tangent;
        if (i == 0) {p.pos = start_pos;}
        else
        {
            Vector pos = cloud[i-1].pos + (tangent * interval);
            p.pos = pos;
        }
        cloud[i] = p;
    }
    // generate wisp map
    for (int i = 0; i < particle_num; ++i)
    {
        WispParms* wispParameters = new WispParms;
        FractalSum<PerlinNoiseGustavson>* FSPN1 = new FractalSum<PerlinNoiseGustavson>;
        FractalSum<PerlinNoiseGustavson>* FSPN2 = new FractalSum<PerlinNoiseGustavson>;
        wispParameters->FSPN1 = FSPN1;
        wispParameters->FSPN2 = FSPN2;
        Noise_t FSPN1Parms;
        Noise_t FSPN2Parms;
        FSPN1Parms.translate = cloud[i].pos;
        wispParameters->FSPN1->setParameters(FSPN1Parms);
        wispParameters->FSPN2->setParameters(FSPN2Parms);
        wispParmsMap[i] = wispParameters;
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
                    FSPN1Parms.frequency = 0.8531;
                    FSPN1Parms.octaves = 1.5;
                    // FSPN1Parms.translate = cloud[i].tangent + cloud[i-1].pos;
                    // FSPN2
                    FSPN2Parms.octaves = 2.2;
                    FSPN2Parms.frequency = 2.5343;
                    FSPN2Parms.fjump = 2.5;
                    FSPN1Parms.pscale = 1.5f;
                    // update parms
                    wispParameters->FSPN1->setParameters(FSPN1Parms);
                    wispParameters->FSPN2->setParameters(FSPN2Parms);

                    wispParameters->delta_x = 1.5;
                    wispParameters->dscale = 0.5;
                    wispParameters->opacity = 0.1f * float(pow(young, 5.0));
                    wispParameters->dot_num = 1000000;
                    wispParameters->clump = 1.5f;
                    wispParameters->offset = -cloud[i].tangent * 4.0;

                    break;
                }

                case 1:
                {
                    WispParms* wispParameters = wispParmsMap[i];
                    float young = (cloud[i].lifetime - cloud[i].age) / float(cloud[i].lifetime);
                    float old = cloud[i].age / float(cloud[i].lifetime);
                    Noise_t FSPN1Parms = wispParameters->FSPN1->getNoiseParameters();
                    Noise_t FSPN2Parms = wispParameters->FSPN2->getNoiseParameters();

                    FSPN1Parms.P = cloud[i].pos + cloud[i].tangent * interval;
                    FSPN1Parms.translate = cloud[i].pos;
                    FSPN2Parms.frequency = 4.5343;
                    FSPN2Parms.fjump = 2.5;
                    FSPN2Parms.octaves = 2.4;
                    FSPN2Parms.translate = cloud[i].pos;
                    wispParameters->dot_num = 500000;

                    wispParameters->clump = 0.233f;
                    FSPN1Parms.pscale = 1.2f * old;

                    // update parms
                    wispParameters->FSPN1->setParameters(FSPN1Parms);
                    wispParameters->FSPN2->setParameters(FSPN2Parms);

                    wispParameters->opacity = 0.005f;
                    wispParameters->delta_x = 1.5;
                    wispParameters->dscale = 0.5;

                    break;
                }

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
            std::cout << "opacity: " << wispParmsMap[i]->opacity << std::endl;
            float pscale = wispParmsMap[i]->FSPN1->getNoiseParameters().pscale;
            std::cout << "pscale: " << pscale << std::endl;
            SingleGuideWisp wisp(*wispParmsMap[i], wispGrid);
        }
    }
}
