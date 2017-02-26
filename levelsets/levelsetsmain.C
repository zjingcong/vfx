
# include <iostream>
# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <vector>

# include "Vector.h"
# include "PolyModel.h"
# include "Types.h"
# include "Grid.h"
# include "Levelsets.h"

using namespace std;
using namespace lux;


int main(int argc, char* argv[])
{
	string bunnyPath = "./models/bunny.obj";
	// load bunny model
	PolyModel polyBunny;
	polyBunny.loadObj(bunnyPath);
	// generate bunny levelsets
	PolyLevelsets bunnyLevelsets(polyBunny, 3, 0.004);
	FloatGrid::Ptr bunnyGrid = bunnyLevelsets.getLevelsets();

	/*
	// test
	for (std::vector<Face>::iterator it = polyBunny.begin(); it != polyBunny.end(); ++it)
	{
		Face f = *it;
		Point p = f.getPoint(0);
		Vector pos0 = p.getPos();
		cout << pos0.X() << " " << pos0.Y() << " " << pos0.Z() << endl;
	}
	*/

	/*
	// test signed distance
	Face f = polyBunny[0];
	Point p = f.getPoint(1);
	Vector pos0 = p.getPos();
	cout << pos0.X() << " " << pos0.Y() << " " << pos0.Z() << endl;

	float d = f.getSignDistance(pos0);
	cout << "distance: " << d << endl;
	*/

	/*
	// test polyLevelsets
	// create a sparse grid
	FloatGrid::Ptr grid = FloatGrid::create();
	// set grid voxel size	
	Transform::Ptr trans;
	trans = Transform::createLinearTransform(0.01);
	grid -> setTransform(trans);

	polyLevelsets(*grid, polyBunny.polyFaces);
	*/

	/*
	// test inter
	FloatGrid::Ptr grid;
	// grid = FloatGrid::create(0.1);
	grid = openvdb::createLevelSet<FloatGrid>(0.01, 10);
	grid -> setBackground(-1000);
	// FloatGridVolume gridVolumef(*grid);
	// Vector x(0.0, 0.0, 0.0);
	// float value = gridVolumef.eval(x);
	// cout << "value: " << value << endl;
	FloatGrid::Accessor accessor = grid -> getAccessor();
	Coord ijk(1, 0, 0);
	float value = accessor.getValue(ijk);
	cout << "value: " << value << endl;
	*/

	return 0;
}

