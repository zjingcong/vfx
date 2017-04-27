# ifndef __WISP_H__
# define __WISP_H__

# include "Types.h"
# include "Noise.h"
# include "PerlinNoise.h"

using namespace lux;


// wisp parms
struct WispParms
{
    WispParms():
        corr(0.0),
        opacity(0.05),
        dscale(2.0),
        clump(0.0),
        delta_x(5),
        dot_num(5000000),
        offset(Vector(0.0, 0.0, 0.0)),
        scale1(Vector(1.0, 1.0, 1.0))
        {}

    float corr;
    float opacity;
    float dscale;
    float clump;
    float delta_x;
    int dot_num;
    Vector offset;  // random location offset
    Vector scale1;  // scale for FSPN1
    Noise* FSPN1;   // guide particle
    Noise* FSPN2;
};


class SingleGuideWisp
{
    public:
        // create a new grid
        SingleGuideWisp(WispParms& w, float s);
        // burn dot on a given grid
        SingleGuideWisp(WispParms& w, FloatGrid::Ptr g);
        ~SingleGuideWisp() {}

        FloatGrid::Ptr getWispGrid()    {return wispGrid;}
        BBox getBBox()  {return wispBBox;}

    private:
        WispParms& wisp_parms;
        Noise* guideParticle;   // guide particle
        Noise* FSPN2;
        int dot_num;
        Noise_t FSPN1Parms;
        Noise_t FSPN2Parms;
        FloatGrid::Ptr wispGrid;
        Vec3s voxelSize;
        Transform::Ptr xform;
        BBox wispBBox;

        void createDot();
        void burnDot(Vector xw);
        void createWispDots();
};

# endif
