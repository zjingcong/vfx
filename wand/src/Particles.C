# include "Particles.h"

WispCloud::WispCloud(int num, float i, Vector sp): particle_num(num), interval(i), start_pos(sp)
{
    // test interval and sp here
    interval = 0.8;
    start_pos = Vector(0.0, 0.0, 12.0);

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
        p.lifetime = 6;
        p.isActive = true;
        // generate position and tangent
        Vector hemisphereRandVec = randm::hemisphereRand(i);
        Vector tangentRand = Vector(hemisphereRandVec.X(), hemisphereRandVec.Z(), -hemisphereRandVec.Y());
        Vector tangent = (tangentRand + Vector(0.0, 0.0, -1.0)).unitvector();
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
        Noise_t FSPN1Parms;
        FSPN1Parms.P = cloud[i].pos;
        Noise_t FSPN2Parms;
        FSPN2Parms.octaves = 2.2;
        FSPN2Parms.frequency = 2.5343;
        FSPN2Parms.fjump = 2.5;
        FSPN2Parms.translate = cloud[i].pos;
        wispParameters->FSPN1 = FSPN1;
        wispParameters->FSPN2 = FSPN2;
        wispParameters->FSPN1->setParameters(FSPN1Parms);
        wispParameters->FSPN2->setParameters(FSPN2Parms);
        wispParmsMap[i] = wispParameters;
    }
}


void WispCloud::spendTime(int delta_time)
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
        if (cloud[i].age > 0 && cloud[i].isActive)
        {
            WispParms* wispParameters = wispParmsMap[i];
            float young = (cloud[i].lifetime - cloud[i].age) / float(cloud[i].lifetime);
            float old = cloud[i].age / float(cloud[i].lifetime);
            Noise_t FSPN1Parms = wispParameters->FSPN1->getNoiseParameters();
            FSPN1Parms.frequency = 0.5531;
            FSPN1Parms.pscale = 1.5f * old + interval;
            Noise_t FSPN2Parms = wispParameters->FSPN2->getNoiseParameters();
            FSPN2Parms.octaves = 2.2;
            FSPN2Parms.frequency = 2.5343;
            FSPN2Parms.fjump = 2.5;
            FSPN2Parms.translate += Vector(0.0, 0.0, -1) * interval * delta_time;
            wispParameters->FSPN1->setParameters(FSPN1Parms);
            wispParameters->FSPN2->setParameters(FSPN2Parms);

            wispParameters->clump = 1.5f + young;
            wispParameters->opacity = 0.05f * float(pow(young, 5.0));
            wispParameters->dot_num = 1500000;
            wispParameters->delta_x = 0.5;
            wispParameters->dscale = 0.5;
            wispParameters->exp = young * 0.8f;
            wispParameters->offset = -cloud[i].tangent * 4.0;
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
