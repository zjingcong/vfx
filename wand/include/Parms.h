# ifndef __PARMS_H__
# define __PARMS_H__

# include "ConfigParser.h"
# include "Color.h"

// color
# define COLOR_ORANGE 0
# define COLOR_PURPLE 1
# define COLOR_BUNNY 2


using namespace lux;

// volume property
typedef struct
{
    Color matColor;
    Color emColor;
    float rho;
    float K;
}   VolumeConstProperty;

map<int, Color> colorMap;


inline void build_colorMap(int id)
{
    float em_factor = 1.0;
    if (id >= 180 && id <= 340)
    {
        em_factor = float(340 - id) / 160.0;
    }
    else if (id > 340)
    {
        em_factor = 0.0;
    }
    colorMap[COLOR_ORANGE] = Color(15.0 * em_factor, 1.0 * em_factor, 4.0 * em_factor, 1.0);
    colorMap[COLOR_PURPLE] = Color(4.0 * em_factor, 1.0 * em_factor, 15.0 * em_factor, 1.0);
    colorMap[COLOR_BUNNY] = Color(0.0, 1.0, 1.0, 1.0);
}


inline VolumeConstProperty setVolumeConstProperty(int id)
{
    VolumeConstProperty property;
    property.matColor = Color(0.0, 0.0, 0.0, 0.0);
    property.emColor = Color(0.4, 8.0, 8.0, 1.0);
    float rho_factor = 1.0;
    if (id >= 180 && id <= 360)
    {
        rho_factor = float(360 - id) / 180.0;
    }
    property.rho = 0.3 * rho_factor;
    property.K = 0.4;

    return property;
}


inline VolumeConstProperty setBunnyConstProperty(int id)
{
    VolumeConstProperty property;
    property.matColor = Color(0.2, 0.2, 0.2, 1.0);
    property.emColor = Color(0.0, 0.0, 0.0, 0.0);
    float rho_factor = 1.0;
    if (id >= 113 && id <= 179)
    {
        rho_factor = float(id - 113) / 66.0;
    }
    property.rho = 400 * rho_factor;
    property.K = 0.4;

    return property;
}

# endif
