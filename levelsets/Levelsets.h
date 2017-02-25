# ifndef __LUX_LEVELSETS_H__
# define __LUX_LEVELSETS_H__

# include <set>

# include "PolyModel.h"
# include "Types.h"

namespace lux
{

	class PolyLevelsets
	{
		public:
			PolyLevelsets(PolyModel poly, int w, float s);
			~PolyLevelsets()	{}

			FloatGrid::Ptr getLevelsets();

		private:
			PolyModel polyModel;
			int halfWidth;
			float voxelSize;

			FloatGrid::Ptr myGrid;	// levelsets grid
			Transform::Ptr xform;	// levelsets grid's transform
			FloatGrid::Accessor accessor;	// accessor of the levelsets grid

			std::set<Coord> positiveCoordSet;	// a set containing positive grid points

			void createFaceLevelsets(Face face);
			void createLevelsets();
	};

}

# endif

