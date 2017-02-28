# ifndef __LUX_GRID_H__
# define __LUX_GRID_H__

# include "Volume.h"
# include "Types.h"
# include "Vector.h"

namespace lux
{

	// convert a float grid to a float volume
	class FloatGridVolume: public Volume<float>
	{
		public:
			FloatGridVolume(FloatGrid::Ptr grid): myFloatGrid(grid)	{}
			~FloatGridVolume()	{}

			const float eval(const Vector& x) const;

		private:
			FloatGrid::Ptr myFloatGrid;
	};


	// stamp a float volume to a float grid
	class FloatVolumeToGrid
	{
		public:
			FloatVolumeToGrid(Volume<float>& f, float s, BBox& bbox);
			~FloatVolumeToGrid()	{}

			FloatGrid::Ptr getVolumeGrid();

		private:
			Volume<float>& myVolume;
			float voxelSize;
			BBox volumeBBox;

			FloatGrid::Ptr myGrid;
			Transform::Ptr transform;

			void createVolumeGrid();
	};

}

# endif

