
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
	// generate bunny volume
	FloatGridVolume bunnyVolume(*bunnyGrid);
	Vector x(0.05, 0.01, 0.003);
	float value = bunnyVolume.eval(x);
	cout << "value: " << value << endl;

	return 0;
}

