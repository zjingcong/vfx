# ifndef __PARTICLES_H__
# define __PARTICLES_H__

# include <vector>

# include "Noise.h"
# include "Grid.h"
# include "Wisp.h"
# include "Volume.h"
# include "Vector.h"


using namespace lux;


struct Particle
{
    Particle():
        id(-1),
        lifetime(0),
        age(-1),
        isActive(false)
    {}

    int id;
    int lifetime;
    int age;
    bool isActive;
};

typedef std::map<int, Particle> Cloud; // a collection of particles


class WispCloud
{
    public:
        WispCloud(int num);
        ~WispCloud()    {}

        void spendTime(int time = 1);
        void stampWispCloudGrid(FloatGrid::Ptr wispGrid);
        void testSys();

    private:
        int particle_num;

        std::map<int, WispParms> wispParmsMap;
        Cloud cloud;

        void initialize();
};

# endif
