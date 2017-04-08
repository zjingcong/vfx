# ifndef __LUX_LEVELSETS_H__
# define __LUX_LEVELSETS_H__

# include <set>

# include "Grid.h"
# include "PolyModel.h"
# include "Types.h"

namespace lux
{

    typedef FloatGrid::Ptr VDBLevelsetsPtr;

	class PolyLevelsets
	{
		public:
			// gridBack should be negative
			// default gridBack is -1000
			PolyLevelsets(PolyModel poly, float s, int w = 3): polyModel(poly), voxelSize(s), halfWidth(w) {}
			~PolyLevelsets()	{}
			
			FloatGrid::Ptr getLevelsets();
            VDBLevelsetsPtr getVDBLevelsets();

			BBox getBBox()  {return levelsetsBBox;}

		private:
			bool isVDB;
			PolyModel polyModel;
			float voxelSize;
            int halfWidth;

            float backgroundValue;
            BBox levelsetsBBox;

			FloatGrid::Ptr VDBcreateLevelsets();    // use openvdb meshToLevelSet to generate levelsets
	};

}

# endif

