# include <vector>

# include "Levelsets.h"
# include "Vector.h"

# define myMax(x, y) (x > y ? x : y)
# define myMin(x, y) (x < y ? x : y)


PolyLevelsets::PolyLevelsets(PolyModel poly, int w, float s)
{
	polyModel = poly;
	halfWidth = w;
	voxelSize = s;

	// create a new grid of FloatGrid classified as a "Level Set"
	myGrid = createLevelSet<FloatGrid>(voxelSize, halfWidth);
	// get levelsets grid's transform
	xform = myGrid -> transformPtr();
	// get levelsets grid's accessor
	accessor = myGrid -> getAccessor();

	// set background
	myGrid -> setBackground(-1000);
}


void PolyLevelsets::createFaceLevelsets(Face face)
{
	// get face points position in world space
	lux::Vector pos0 = face.getPoint(0).getPos();
	lux::Vector pos1 = face.getPoint(1).getPos();
	lux::Vector pos2 = face.getPoint(2).getPos();
	Vec3s xyz0(pos0.X(), pos0.Y(), pos0.Z());
	Vec3s xyz1(pos1.X(), pos1.Y(), pos1.Z());
	Vec3s xyz2(pos2.X(), pos2.Y(), pos2.Z());
	// convert the world space positions to the index space coordinates
	Coord ijk0 = xform -> worldToIndex(xyz0);
	Coord ijk1 = xform -> worldToIndex(xyz1);
	Coord ijk2 = xform -> worldToIndex(xyz2);
	// get bounding box
	Int32 tmp;
	Int32 i_min, j_min, k_min;
	Int32 i_max, j_max, k_max;
	// get bounding box LLC
	tmp = myMin(ijk0.x(), ijk1.x());
	i_min = myMin(tmp, ijk2.x());
	tmp = myMin(ijk0.y(), ijk1.y());
	j_min = myMin(tmp, ijk2.y());
	tmp = myMin(ijk0.z(), ijk1.z());
	k_min = myMin(tmp, ijk2.z());
	// get bounding box URC
	tmp = myMax(ijk0.x(), ijk1.x());
	i_max = myMax(tmp, ijk2.x()) + 1;
	tmp = myMax(ijk0.y(), ijk1.y());
	j_max = myMax(tmp, ijk2.y()) + 1;
	tmp = myMax(ijk0.z(), ijk1.z());
	k_max = myMax(tmp, ijk2.z()) + 1;
	// expand bounding box to satisfy narrow band
	Int32 delta;
	if ((i_max - i_min) < halfWidth * 2)
	{
		delta = halfWidth - ((i_max - i_min) / 2);
		i_min = i_min - delta;
		i_max = i_max + delta;
	}
	if ((j_max - j_min) < halfWidth * 2)
	{
		delta = halfWidth - ((j_max - j_min) / 2);
		j_min = j_min - delta;
		j_max = j_max + delta;
	}
	if ((k_max - k_min) < halfWidth * 2)
	{
		delta = halfWidth - ((k_max - k_min) / 2);
		k_min = k_min - delta;
		k_max = k_max + delta;
	}
	// traverse the grid points in the bounding box
	for (Int32 i  = i_min; i <= i_max; ++i)
	{
		for (Int32 j = j_min; j <= j_max; ++j)
		{
			for (Int32 k = k_min; k <= k_max; ++k)
			{
				// convert the world space position of the grid point index space coordinate
				Coord gridPointCoord(i, j, k);
				Vec3s gridPointPos = xform -> indexToWorld(gridPointCoord);
				lux::Vector gridPointPosVec(gridPointPos.x(), gridPointPos.y(), gridPointPos.z());
				// get the signed distance to the face
				float signDis = face.getSignDistance(gridPointPosVec);
				// save the signed distance to the grid point value
				// if the previous grid point value is not assigned, then save the distance
				// else if the distance is smaller than previous grid point value, 
				// then refresh the grid point value
				const float gridBack = myGrid -> background();
				if (accessor.getValue(gridPointCoord) == gridBack)
				{
					accessor.setValue(gridPointCoord, signDis);
					// if the signed distance is positive, add the point to positiveCoordSet
					if (signDis > 0)	{positiveCoordSet.insert(gridPointCoord);}
				}
				else if (signDis < accessor.getValue(gridPointCoord))
					{accessor.setValue(gridPointCoord, signDis);}
			}
		}
	}
	// end of traversing
}


void PolyLevelsets::createLevelsets()
{ 
	// traverse all the faces of the polymodel
	for (std::vector<Face>::iterator iter = polyBunny.polyFaces.begin(); 
	     iter != polyBunny.polyFaces.end(); 
	     ++iter)
	{
		Face f = *iter;
		// within the narrow band, calculate the signed distance
		createFaceLevelsets(f);
		// for grid points interior to narrow band, set values to +1000
		// traverse every point in positive set
		for (std::set<Coord>::iterator iter = positiveCoordSet.begin(); 
		     iter != positiveCoordSet.end(); 
		     ++iter)
		{
			Coord ijk = *iter;
			Int32 i0 = ijk.x();
			Int32 j0 = ijk.y();
			Int32 k0 = ijk.z();
			// find the neighbor of the point
			for (Int32 i = i0 - 1; Int32 i = i0 + 1; ++i)
			{
				for (Int32 j = j0 - 1; Int32 j = j0 + 1; ++j)
				{
					for (Int32 k = k0 -1; Int32 k = k0 + 1; ++k)
					{
						Coord neighborCoord(i, j, k);
						// if the neighbor is unassigned
						if (accessor.getValue(gridPointCoord) == gridBack)
						{
							// set the value to +1000
							accessor.setValue(gridPointCoord, 1000);
							// add neighbor to positive set
							positiveCoordSet.insert(neighborCoord);
						}
					}
				}
			}
			// end of neighbor finding
		}
		// end of traversing
	}
}


// return the polymodel levelsets
FloatGrid::Ptr getLevelsets()
{
	createLevelsets();

	return myGrid;
}

