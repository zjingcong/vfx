# include "Particles.h"

WispCloud::WispCloud(int num): particle_num(num)
{
    initialize();
}


void WispCloud::initialize()
{
    // generate wisp map
    for (int i = 0; i < particle_num; ++i)
    {
        WispParms wispParameters;
        FractalSum<PerlinNoiseGustavson>* FSPN1 = new FractalSum<PerlinNoiseGustavson>;
        FractalSum<PerlinNoiseGustavson>* FSPN2 = new FractalSum<PerlinNoiseGustavson>;
        Noise_t FSPN1Parms;
        Noise_t FSPN2Parms;
        FSPN1Parms.P = Vector(0.0, 0.0, i);
        FSPN2Parms.octaves = 1.9;
        FSPN2Parms.frequency = 1.5343;
        FSPN2Parms.fjump = 2.5;
        wispParameters.clump = 2.0;
        wispParameters.FSPN1 = FSPN1;
        wispParameters.FSPN2 = FSPN2;
        wispParameters.dot_num = 5000000;
        wispParameters.offset = Vector(0.0, 0.0, 0.0);

        FSPN1->setParameters(FSPN1Parms);
        FSPN2->setParameters(FSPN2Parms);

        wispParmsMap[i] = wispParameters;
    }
    // generate particle map
    for (int i = 0; i < particle_num; ++i)
    {
        Particle p;
        p.id = i;
        p.age = -i;
        p.lifetime = 10;
        p.isActive = true;
        cloud[i] = p;
    }
}


void WispCloud::spendTime(int delta_time)
{
    for (int i = 0; i < particle_num; ++i)
    {
        if (cloud[i].isActive)
        {
            cloud[i].age += delta_time;
            if (cloud[i].age >= cloud[i].lifetime)  {cloud[i].isActive = false;}    // dead
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
            SingleGuideWisp wisp(wispParmsMap[i], wispGrid);
        }
    }
}
