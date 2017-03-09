# include <vector>
# include <iostream>
# include "math.h"

# include <openvdb/tools/MeshToVolume.h>

# include "Levelsets.h"
# include "Vector.h"

# define myMax(x, y) (x > y ? x : y)
# define myMin(x, y) (x < y ? x : y)

using namespace lux;


PolyLevelsets::PolyLevelsets(bool isvdb, PolyModel poly, int w, float s, float back)
{
	isVDB = isvdb;
	polyModel = poly;
	halfWidth = w;
	voxelSize = s;
	gridBack = back;

	if (!isVDB)
	{
		// create a new grid of FloatGrid classified as a "Level Set"
		myGrid = openvdb::createLevelSet<FloatGrid>(voxelSize, halfWidth);
		// get levelsets grid's transform
		xform = myGrid -> transformPtr();
		// set grid background
		myGrid -> setBackground(gridBack);
	}
}


void PolyLevelsets::createFaceLevelsets(Face face)
{
	// get levelsets grid's accessor
	FloatGrid::Accessor accessor = myGrid -> getAccessor();
	// get face points position in world space
	lux::Vector pos0 = face.getPoint(0).getPos();
	lux::Vector pos1 = face.getPoint(1).getPos();
	lux::Vector pos2 = face.getPoint(2).getPos();
	Vec3s xyz0(pos0.X(), pos0.Y(), pos0.Z());
	Vec3s xyz1(pos1.X(), pos1.Y(), pos1.Z());
	Vec3s xyz2(pos2.X(), pos2.Y(), pos2.Z());
	// convert the world space positions to the index space coordinates
	Coord ijk0 = xform -> worldToIndexNodeCentered(xyz0);
	Coord ijk1 = xform -> worldToIndexNodeCentered(xyz1);
	Coord ijk2 = xform -> worldToIndexNodeCentered(xyz2);
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
	if (halfWidth != 0)
	{
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
				if (accessor.getValue(gridPointCoord) == gridBack)
				{
					accessor.setValue(gridPointCoord, signDis);
					accessor.setValueOn(gridPointCoord);
				}
				else if (abs(signDis) < abs(accessor.getValue(gridPointCoord)))
				// else if (signDis < accessor.getValue(gridPointCoord))
					{accessor.setValue(gridPointCoord, signDis);}
			}
		}
	}
	// end of traversing
}


// use all the positive points as a lp
void PolyLevelsets::createLevelsets_all()
{
	// get levelsets grid's accessor
	FloatGrid::Accessor accessor = myGrid -> getAccessor();
	// traverse all the faces of the polymodel
	for (std::vector<Face>::iterator iter = polyModel.polyFaces.begin(); 
	     iter != polyModel.polyFaces.end(); 
	     ++iter)
	{
		Face f = *iter;
		// within the narrow band, calculate the signed distance
		createFaceLevelsets(f);
	}

	// --------------------------------------------------------------------

	// for grid points interior to narrow band, set values to +1000
	bool noneighborFlag = false;
	while (!noneighborFlag)
	{
		int unassigned_num = 0;

		int points_num = 0;
		int point0num = 0;
		int positive_num = 0;
		int negative_num = 0;

		for (FloatGrid::ValueOnIter iter = myGrid -> beginValueOn(); iter; ++iter)
		{
			// cout << iter.getCoord() << ": " << *iter << endl;
			points_num++;
			if (*iter == 0) {point0num++;}
			if (*iter < 0)	{negative_num++;}
			if (*iter > 0)	{positive_num++;}
		}
		cout << "points_num: " << points_num << endl;
		cout << "point0num: " << point0num << endl;
		cout << "negative_num: " << negative_num << endl;
		cout << "positive_num: " << positive_num << endl;
		
		// a new set to store new positive points
		std::set<Coord> newpositiveCoordSet;
		// traverse every point in positive set		
		for (FloatGrid::ValueOnIter iter = myGrid -> beginValueOn(); iter; ++iter)
		{
			// get positive point
			if (*iter > 0)
			{
				Coord ijk = iter.getCoord();
				Int32 i0 = ijk.x();
				Int32 j0 = ijk.y();
				Int32 k0 = ijk.z();
				// find the neighbor of the point
				for (Int32 in = i0 - 1; in <= i0 + 1; ++in)
				{
					for (Int32 jn = j0 - 1; jn <= j0 + 1; ++jn)
					{
						for (Int32 kn = k0 -1; kn <= k0 + 1; ++kn)
						{
							Coord neighborCoord(in, jn, kn);
							// if the neighbor is unassigned, add the neighbor to new positive set
							if (accessor.getValue(neighborCoord) == gridBack)
							{
								unassigned_num++;
								newpositiveCoordSet.insert(neighborCoord);
							}
						}
					}
				}	// end of neighbor finding
			}
		}	// end of traversing

		// set all new positive points value
		for (std::set<Coord>::iterator it = newpositiveCoordSet.begin();
		     it != newpositiveCoordSet.end(); ++it)
			{accessor.setValue(*it, -gridBack);}

		newpositiveCoordSet.clear();
		// set the flag to stop while loop
		cout << "unassigned_num: " << unassigned_num << endl; 
		if (unassigned_num == 0)	{noneighborFlag = true;}

		cout << "-----------------------------------------------------------------" << endl;

	}	// end of while
}


// use new positive points as a lp
void PolyLevelsets::createLevelsets()
{
	// get levelsets grid's accessor
	FloatGrid::Accessor accessor = myGrid -> getAccessor();
	// traverse all the faces of the polymodel
	for (std::vector<Face>::iterator iter = polyModel.polyFaces.begin(); 
	     iter != polyModel.polyFaces.end(); 
	     ++iter)
	{
		Face f = *iter;
		// within the narrow band, calculate the signed distance
		createFaceLevelsets(f);
	}

	// ----------------------------------------------------------------------


	// get initial positive points list
	std::set<Coord> positiveCoordSet;
	for (FloatGrid::ValueOnIter iter = myGrid -> beginValueOn(); iter; ++iter)
	{
		if (*iter > 0)
		{positiveCoordSet.insert(iter.getCoord());}
	}

	// for grid points interior to narrow band, set values to +1000
	bool noneighborFlag = false;
	for (int i  = 0; i < 5; i++)	// test here
	// while (!noneighborFlag)
	{
		int unassigned_num = 0;

		/// ============================ test ==================================
		int points_num = 0;
		int point0num = 0;
		int positive_num = 0;
		int negative_num = 0;
		int current_positive = 0;
		for (FloatGrid::ValueOnIter iter = myGrid -> beginValueOn(); iter; ++iter)
		{
			// cout << iter.getCoord() << ": " << *iter << endl;
			points_num++;
			if (*iter == 0) {point0num++;}
			if (*iter < 0)	{negative_num++;}
			if (*iter > 0)	{positive_num++;}
		}
		cout << "points_num: " << points_num << endl;
		cout << "point0num: " << point0num << endl;
		cout << "negative_num: " << negative_num << endl;
		cout << "positive_num: " << positive_num << endl;
		for (std::set<Coord>::iterator iter = positiveCoordSet.begin(); 
		     iter != positiveCoordSet.end(); ++iter)
		{
			current_positive++;
		}
		cout << "current_positive_num: " << current_positive << endl;
		/// ============================ test ==================================

		// a new set to store new positive points
		std::set<Coord> newpositiveCoordSet;
		// traverse every point in positive set		
		for (std::set<Coord>::iterator iter = positiveCoordSet.begin(); 
		     iter != positiveCoordSet.end(); ++iter)
		{
			// get positive point
			Coord ijk = *iter;
			Int32 i0 = ijk.x();
			Int32 j0 = ijk.y();
			Int32 k0 = ijk.z();
			// find the neighbor of the point
			for (Int32 in = i0 - 1; in <= i0 + 1; ++in)
			{
				for (Int32 jn = j0 - 1; jn <= j0 + 1; ++jn)
				{
					for (Int32 kn = k0 -1; kn <= k0 + 1; ++kn)
					{
						Coord neighborCoord(in, jn, kn);
						// if the neighbor is unassigned, add the neighbor to new positive set
						if (accessor.getValue(neighborCoord) == gridBack)
						{
							unassigned_num++;
							newpositiveCoordSet.insert(neighborCoord);
						}
					}
				}
			}	// end of neighbor finding
		}	// end of traversing
		int new_positive_num = 0;
		// set all new positive points value
		for (std::set<Coord>::iterator it = newpositiveCoordSet.begin();
		     it != newpositiveCoordSet.end(); ++it)
		{
			// set positive value
			accessor.setValue(*it, -gridBack);
			new_positive_num++;
		}

		positiveCoordSet.clear();
		positiveCoordSet = newpositiveCoordSet;
		newpositiveCoordSet.clear();
		cout << "new_positive_num: " << new_positive_num << endl;
		cout << "unassigned_num: " << unassigned_num << endl; 
		// set the flag to stop while loop
		if (unassigned_num == 0)	{noneighborFlag = true;}

		cout << "-----------------------------------------------------------------" << endl;
	}	// end of while
}


FloatGrid::Ptr PolyLevelsets::VDBcreateLevelsets()
{
	std::vector<Vec3s> points = polyModel.polyPoints;
	std::vector<Vec3I> triangles = polyModel.triIndices;
	Transform::Ptr xform = Transform::createLinearTransform(voxelSize);
	FloatGrid::Ptr mesh2levelsets = openvdb::tools::meshToLevelSet<FloatGrid>(*xform, points, triangles, halfWidth);

	return mesh2levelsets;
}


// return the polymodel levelsets
FloatGrid::Ptr PolyLevelsets::getLevelsets()
{
	if (isVDB)
		{return VDBcreateLevelsets();}
	else
	{
		createLevelsets();
		// createLevelsets_all();
		return myGrid;
	}
}

