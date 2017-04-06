# ifndef __LUX_LEVELSETS_H__
# define __LUX_LEVELSETS_H__

# include <set>

# include "Grid.h"
# include "PolyModel.h"
# include "Types.h"

namespace lux
{

	class PolyLevelsets
	{
		public:
			// gridBack should be negative
			// default gridBack is -1000
			PolyLevelsets(bool isvdb, PolyModel poly, float s, int w = 3);
			~PolyLevelsets()	{}
			
			FloatGrid::Ptr getLevelsets();
            BBox getBBox()  {return levelsetsBBox;}

		private:
			bool isVDB;
			PolyModel polyModel;
			float voxelSize;
            int halfWidth;

            float backgroundValue;
            BBox levelsetsBBox;
			FloatGrid::Ptr myGrid;	// levelsets grid
			Transform::Ptr xform;	// levelsets grid's transform

			void createFaceLevelsets(Face face);
			void createLevelsets();
			void createLevelsets_all();
			FloatGrid::Ptr VDBcreateLevelsets();    // use openvdb meshToLevelSet to generate levelsets
	};

}

# endif

