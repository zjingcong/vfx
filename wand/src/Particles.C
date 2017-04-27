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
        WispParms* wispParameters = new WispParms;
        FractalSum<PerlinNoiseGustavson>* FSPN1 = new FractalSum<PerlinNoiseGustavson>;
        FractalSum<PerlinNoiseGustavson>* FSPN2 = new FractalSum<PerlinNoiseGustavson>;
        wispParameters->FSPN1 = FSPN1;
        wispParameters->FSPN2 = FSPN2;
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
    // update particle status
    for (int i = 0; i < particle_num; ++i)
    {
        if (cloud[i].isActive)
        {
            cloud[i].age += delta_time;
            if (cloud[i].age >= cloud[i].lifetime)  {cloud[i].isActive = false;}    // dead
        }
    }
    // update wisp parms
    for (int i = 0; i < particle_num; ++i)
    {
        if (cloud[i].age > 0 && cloud[i].isActive)
        {
            WispParms* wispParameters = wispParmsMap[i];
            // do parms update here
            Noise_t* FSPN1Parms = new Noise_t;
            FSPN1Parms->P = Vector(0.0, 0.0, 10 - i);
            FSPN1Parms->frequency = 0.8231;
            FSPN1Parms->pscale = 1.5;
            // FSPN1Parms.fjump = 2.5;
            // FSPN1Parms.roughness = 1.0;
            // FSPN1Parms.octaves = 0.5;
            Noise_t* FSPN2Parms = new Noise_t;
            FSPN2Parms->octaves = 2.2;
            FSPN2Parms->frequency = 2.5343;
            FSPN2Parms->fjump = 2.5;
            wispParameters->FSPN1->setParameters(*FSPN1Parms);
            wispParameters->FSPN2->setParameters(*FSPN2Parms);

            wispParameters->clump = 2.5;
            wispParameters->opacity = 0.05;
            wispParameters->dot_num = 5000000;
            wispParameters->delta_x = 0.5;
            wispParameters->dscale = 0.5;
            wispParameters->offset = Vector(0.0, 0.0, 2.3);
            wispParameters->scale1 = Vector(1.0, 1.0, 2.0);
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
            SingleGuideWisp wisp(*wispParmsMap[i], wispGrid);
        }
    }
}
