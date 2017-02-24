# ifndef __TOOLS_H__
# define __TOOLS_H__

# include <vector>

# include "Types.h"
# include "PolyModel.h"
# include "Vector.h"


// void polyNarrowBandBox(Face face, )

void polyLevelsets(FloatGrid& grid, std::vector<Face>& polyModel)
{
	// get the grid's transform
	Transform::Ptr xform = grid.transformPtr();
	// access to each face of the polygon model
	for (std::vector<Face>::iterator it = polyModel.begin(); it != polyModel.end(); ++it)
	{
		Face f = *it;
		lux::Vector pos0 = f.getPoint(0).getPos();
		lux::Vector pos1 = f.getPoint(1).getPos();
		lux::Vector pos2 = f.getPoint(2).getPos();

		Vec3s xyz0(pos0.X(), pos0.Y(), pos0.Z());
		Vec3s xyz1(pos1.X(), pos1.Y(), pos1.Z());
		Vec3s xyz2(pos2.X(), pos2.Y(), pos2.Z());

		Coord ijk0 = xform -> worldToIndexNodeCentered(xyz0);
		Coord ijk1 = xform -> worldToIndexNodeCentered(xyz1);
		Coord ijk2 = xform -> worldToIndexNodeCentered(xyz2);

		std::cout << ijk0 << " " << ijk1 << " " << ijk2 << endl;
	}
}

# endif

