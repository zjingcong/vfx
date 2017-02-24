
# include <iostream>
# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <list>

# include <openvdb/openvdb.h>

# include "Vector.h"
# include "PolyModel.h"

using namespace std;
using namespace lux;


int main(int argc, char* argv[])
{
	openvdb::initialize();

	std::list<Face> polyBunny;
	string bunnyPath = "./models/bunny.obj";
	load_obj(bunnyPath, polyBunny);
	
	return 0;
}

