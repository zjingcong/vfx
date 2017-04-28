# ifndef __PARTICLES_H__
# define __PARTICLES_H__

# include <vector>

# include "Noise.h"
# include "Grid.h"
# include "Wisp.h"
# include "Volume.h"
# include "Vector.h"
# include "Randomm.h"
# include "Xform.h"


using namespace lux;


struct Particle
{
    Particle():
        id(-1),
        lifetime(0),
        age(-1),
        pos(Vector(0.0, 0.0, 0.0)),
        tangent(Vector(0.0, 0.0, 1.0)),
        isActive(false)
    {}

    int id;
    int lifetime;
    int age;
    Vector pos;
    Vector tangent;
    bool isActive;
};

typedef std::map<int, Particle> Cloud; // a collection of particles


class WispCloud
{
    public:
        WispCloud(int num, float i, Vector sp, Vector dp, float dpf, int seed);
        ~WispCloud()    {}

        void spendTime(int time,  int update_type);
        void stampWispCloudGrid(FloatGrid::Ptr wispGrid);
        void testSys();

    private:
        int particle_num;
        float interval;
        Vector start_pos;
        Vector dir;
        float dp_factor;
        int seed;

        std::map<int, WispParms*> wispParmsMap;
        Cloud cloud;

        void initialize();
};

# endif
