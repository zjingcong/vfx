# ifndef __LUX_GRID_H__
# define __LUX_GRID_H__

# include <openvdb/tools/Interpolation.h>
# include "omp.h"

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


	// convert a Vec3f grid to a vector volume
	class VectorGridVolume: public Volume<Vector>
	{
		public:
			VectorGridVolume(Vec3fGrid::Ptr grid): vectorGrid(grid)	{}
			~VectorGridVolume()	{}

			const Vector eval(const Vector& x) const;
            Vec3fGrid::Ptr getGrid()    {return vectorGrid;}

		private:
			Vec3fGrid::Ptr vectorGrid;
	};

    typedef VectorGridVolume* VectorGridVolumePtr;


	// -------------------------------- Volume to Grid -----------------------------------------

	// stamp a float volume to a float grid
	class FloatVolumeToGrid
	{
		public:
            // create a new grid
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
			ColorVolumeToGrid(Volume<Color>& f, FloatGrid::Ptr floatGrid);
			// transform from float grid and color grid should be the same
			ColorVolumeToGrid(Volume<Color>& f, FloatGrid::Ptr floatGrid, Vec4fGrid::Ptr c);
			~ColorVolumeToGrid()	{}

			Vec4fGrid::Ptr getVolumeGrid();

		private:
			Volume<Color>& myVolume;
			float voxelSize;
			BBox volumeBBox;
			FloatGrid::Ptr floatGrid;

			Vec4fGrid::Ptr myGrid;
			Transform::Ptr transform;

			void createVolumeGrid();
			void maskVolumeGrid();
	};


	// stamp a vector volume to a Vec3f grid
	class VectorVolumeToGrid
	{
		public:
			VectorVolumeToGrid(Volume<Vector>& f, float s, BBox& bbox);
			~VectorVolumeToGrid()	{}

			Vec3fGrid::Ptr getVolumeGrid();
            BBox getBBox();

		private:
			Volume<Vector>& vectorVolume;
			float voxelSize;
			BBox volumeBBox;

			Vec3fGrid::Ptr grid;
			Transform::Ptr transform;

			void createVolumeGrid();
	};

}


// tools
template < typename T >
BBox getGridBBox(typename openvdb::Grid<T>::Ptr grid)
{
    // get bounding box
	int min_i = 100000000;	int max_i = -100000000;
	int min_j = 100000000;	int max_j = -100000000;
	int min_k = 100000000;	int max_k = -100000000;
	Transform::Ptr transform = grid->transformPtr();
	for (typename openvdb::Grid<T>::ValueOnIter iter = grid->beginValueOn(); iter; ++iter)
	{
		Coord ijk = iter.getCoord();
		int i = ijk.x();
		int j = ijk.y();
		int k = ijk.z();
		if (i < min_i)	{min_i = i;}	if (i > max_i)	{max_i = i;}
		if (j < min_j)	{min_j = j;}	if (j > max_j)	{max_j = j;}
		if (k < min_k)	{min_k = k;}	if (k > max_k)	{max_k = k;}
	}
	Coord min(min_i, min_j, min_k);
	Coord max(max_i, max_j, max_k);
	Vec3s min_pos = transform -> indexToWorld(min);
	Vec3s max_pos = transform -> indexToWorld(max);
	BBox bbox = BBox(min_pos, max_pos);

	return bbox;
}

# endif
