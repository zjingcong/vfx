# ifndef __PARMS_H__
# define __PARMS_H__

# include "ConfigParser.h"
# include "Color.h"


using namespace lux;

// volume property
typedef struct
{
    Color matColor;
    Color emColor;
    float rho;
    float K;
}   VolumeConstProperty;


VolumeConstProperty setVolumeConstProperty()
{
    VolumeConstProperty property;
    property.matColor = Color(1.0, 1.0, 1.0, 1.0);
    property.emColor = Color(0.4, 8.0, 8.0, 1.0);
    property.rho = 0.4;
    property.K = 0.4;

    return property;
}

# endif
