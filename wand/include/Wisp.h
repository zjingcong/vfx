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
        offset(Vector(0.0, 0.0, 0.0))
        {}

    float corr;
    float opacity;
    float dscale;
    float clump;
    float delta_x;
    int dot_num;
    Vector offset;  // random location offset
    Noise* FSPN1;   // guide particle
    Noise* FSPN2;
};


class SingleGuideWisp
{
    public:
        // 5 million: 5000000
        SingleGuideWisp(WispParms& w, float s);
        ~SingleGuideWisp() {}

        FloatGrid::Ptr getWispGrid()    {return wispGrid;}
        BBox getBBox()  {return wispBBox;}

    private:
        WispParms& wisp_parms;
        Noise* guideParticle;   // guide particle
        Noise* FSPN2;
        float voxelSize;
        int dot_num;

        Noise_t FSPN1Parms;
        Noise_t FSPN2Parms;
        Vector P;   // guide particle position
        FloatGrid::Ptr wispGrid;
        Transform::Ptr xform;
        BBox wispBBox;

        void createDot();
        void burnDot(Vector xw);
        void createWispDots();
};

# endif
