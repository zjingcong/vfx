# ifndef __LUX_GRID_H__
# define __LUX_GRID_H__

# include "Volume.h"
# include "Types.h"
# include "Vector.h"
# include "Color.h"

namespace lux
{

	// -------------------------------- Grid to Volume -----------------------------------------

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


	// convert a Vec4f grid to a color volume
	class ColorGridVolume: public Volume<Color>
	{
		public:
			ColorGridVolume(Vec4fGrid::Ptr grid): myColorGrid(grid)	{}
			~ColorGridVolume()	{}

			const Color eval(const Vector& x) const;

		private:
			Vec4fGrid::Ptr myColorGrid;
	};


	// -------------------------------- Volume to Grid -----------------------------------------

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


	// stamp a color volume to a Vec4f grid
	class ColorVolumeToGrid
	{
		public:
			ColorVolumeToGrid(Volume<Color>& f, float s, BBox& bbox);
			~ColorVolumeToGrid()	{}

			Vec4fGrid::Ptr getVolumeGrid();

		private:
			Volume<Color>& myVolume;
			float voxelSize;
			BBox volumeBBox;

			Vec4fGrid::Ptr myGrid;
			Transform::Ptr transform;

			void createVolumeGrid();
	};

}

# endif

