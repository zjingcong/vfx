
# include <iostream>
# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <list>

# include "Vector.h"
# include "ModelLoad.h"

using namespace std;
using namespace lux;


int main(int argc, char* argv[])
{
	std::list<Face> polyBunny;
	string bunnyPath = "./models/bunny.obj";
	load_obj(bunnyPath, polyBunny);
	
	return 0;
}

