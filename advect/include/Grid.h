# ifndef __LUX_GRID_H__
# define __LUX_GRID_H__

# include <openvdb/tools/Interpolation.h>

# include "Volume.h"
# include "Types.h"
# include "Vector.h"
# include "Color.h"

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)


# define LIGHT_STAMP 1
# define DENSITY_STAMP 0


namespace lux
{

	// -------------------------------- Grid to Volume -----------------------------------------

	// convert a vdb levelset grid to a float volume
	class VDBLevelsetsVolume: public Volume<float>
	{
		public:
			VDBLevelsetsVolume(FloatGrid::Ptr grid): myFloatGrid(grid)	{}
			~VDBLevelsetsVolume()	{}

			const float eval(const Vector& x) const;

		private:
			FloatGrid::Ptr myFloatGrid;
	};


	// convert a float grid to a float volume
	class FloatGridVolume: public Volume<float>
	{
		public:
			FloatGridVolume(FloatGrid::Ptr g);
			~FloatGridVolume()	{}

			const float eval(const Vector& x) const;
            FloatGrid::Ptr getGrid()    {return grid;}

		private:
			FloatGrid::Ptr grid;
    };
	typedef FloatGridVolume* FloatGridVolumePtr;


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
            // tag = 0: density stamping
            // tag = 1: light stamping
			FloatVolumeToGrid(Volume<float>& f, float s, BBox& bbox, int tag = DENSITY_STAMP);
			~FloatVolumeToGrid()	{}

			FloatGrid::Ptr getVolumeGrid(float bg = 0.0);
			BBox getBBox()	{return gridBBox;}

		private:
			Volume<float>& myVolume;
			float voxelSize;
			BBox volumeBBox;
            float background;
            int tag;

			FloatGrid::Ptr myGrid;
			Transform::Ptr transform;
            BBox gridBBox;

			void createVolumeGrid(float bg);
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


// tools
BBox getGridBBox(FloatGrid::Ptr grid);

# endif
