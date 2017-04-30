# ifndef __PATH_H__
# define __PATH_H__

# include <vector>
# include <cmath>

# include "Vector.h"
# include "Xform.h"


struct Guide
{
    Guide():
        pos(0.0, 0.0, 0.0),
        tangent(0.0, 1.0, 0.0),
        distance(0.0)
    {}

    Vector pos;
    Vector tangent; // tangent(i): p(i-1) -> pi
    float distance; // distance(i): p(i-1) -> pi
};

typedef std::vector<Guide> Guideline;


class GuidePath
{
    public:
        GuidePath(std::vector<Vector> g): guides(g)
        {
            Guide p;
            p.pos = guides[0];
            guideline.push_back(p);
            for (int i = 1; i < guides.size(); ++i)
            {
                p.pos = guides[i];
                Vector pre_pos = guideline[i-1].pos;
                Vector tangent = (p.pos - pre_pos).unitvector();
                float d = (p.pos - pre_pos).magnitude();

                p.tangent = tangent;
                p.distance = d;
                guideline.push_back(p);
            }
        }
        ~GuidePath()    {}

        Guideline getGuideline()    {return guideline;}

    private:
        std::vector<Vector> guides;
        Guideline guideline;
};


inline std::vector<Vector> generateHalfCirclePath(float interval, float r, Vector c, Vector axis)
{
    std::vector<Vector> path;
    int num = M_PI * r / interval;
    for (int i = 0; i < num; ++i)
    {
        float x = r;
        float z = pow(r * r - x * x, 0.5f);
        Vector v(x, 0.0, z);
        double theta_degree = acos(Vector(0.0, 1.0, 0.0) * axis) * 180.0 / M_PI;
        Vector a = axis ^ Vector(0.0, 1.0, 0.0);
        v = vecRotation(v, a, float(theta_degree));
        v = v + c;
        path.push_back(v);
    }

    return path;
}


# endif
