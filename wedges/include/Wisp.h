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
        opacity(1.0),
        dscale(1.0),
        clump(0.0),
        delta_x(0.01)
        {}

    float corr;
    float opacity;
    float dscale;
    float clump;
    float delta_x;
    Noise* FSPN2;
};


class SingleGuideWisp
{
    public:
        // 5 million: 5000000
        SingleGuideWisp(Noise& n, WispParms& w, float s, int num);
        ~SingleGuideWisp() {}

        FloatGrid::Ptr getWispGrid();

    private:
        Noise& guideParticle;   // guide particle
        WispParms& wisp_parms;
        float voxelSize;
        int dot_num;

        FloatGrid::Ptr myGrid;
        Transform::Ptr xform;

        void createDot();
        void burnDot(Vector xw);
        void createWispDots();
};

# endif
