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


inline void build_colorMap()
{
    colorMap[COLOR_ORANGE] = Color(10.0, 0.8, 2.0, 1.0);
    colorMap[COLOR_PURPLE] = Color(2.0, 0.8, 10.0, 1.0);
    colorMap[COLOR_BUNNY] = Color(8.0, 8.0, 8.0, 1.0);
}


inline VolumeConstProperty setVolumeConstProperty()
{
    VolumeConstProperty property;
    property.matColor = Color(0.0, 0.0, 0.0, 0.0);
    property.emColor = Color(0.4, 8.0, 8.0, 1.0);
    property.rho = 0.4;
    property.K = 0.4;

    return property;
}


inline VolumeConstProperty setBunnyConstProperty()
{
    VolumeConstProperty property;
    property.matColor = Color(0.2, 0.2, 0.2, 1.0);
    property.emColor = Color(0.0, 0.0, 0.0, 0.0);
    property.rho = 900;
    property.K = 0.4;

    return property;
}

# endif
