
# ifndef __STRUCTS_H__
# define __STRUCTS_H__

# include "Volume.h"
# include "Types.h"


typedef struct
{
	VolumeFloatPtr densityVolume;
	VolumeColorPtr emColorVolume;
	VolumeColorPtr matColorVolume;
	float K;
	BBox bbox;
} modelStruct;

# endif

