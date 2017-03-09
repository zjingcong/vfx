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
			// gridBack should be negative
			// default gridBack is -1000
			PolyLevelsets(bool isvdb, PolyModel poly, int w, float s, float back = -1000);
			~PolyLevelsets()	{}
			
			FloatGrid::Ptr getLevelsets();

		private:
			bool isVDB;
			PolyModel polyModel;
			int halfWidth;
			float voxelSize;
			float gridBack;

			FloatGrid::Ptr myGrid;	// levelsets grid
			Transform::Ptr xform;	// levelsets grid's transform

			void createFaceLevelsets(Face face);
			void createLevelsets();
			void createLevelsets_all();
			// use openvdb meshToLevelSet to generate levelsets
			FloatGrid::Ptr VDBcreateLevelsets();
	};

}

# endif

