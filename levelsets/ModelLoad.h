# include <fstream>
# include <iostream>
# include <string>
# include <sstream>
# include <list>
# include <vector>

# include "Vector.h"

using namespace std;
using namespace lux;


// face struct
typedef struct
{
	Vector p0_pos;
	Vector p1_pos;
	Vector p2_pos;
} Face;


// obj file parser
// v position0, position1, position2
// f p0, p1, p2
void load_obj(string filePath, std::list<Face> polyBunny)
{
	cout << "Load model " << filePath << "..." << endl;
	std::vector<lux::Vector> modelPoints;
	const char* file_path = filePath.c_str();
	ifstream modelFile(file_path);
	string line;
	int vertex_num = 0;
	int face_num = 0;
	while (getline(modelFile, line))
	{
		istringstream iss(line);
		string tag;
		float a, b, c;
		if (iss >> tag >> a >> b >> c)
		{
			// parse the vertices
			if (tag == "v")
			{
				Vector point_pos(a, b, c);
				modelPoints.push_back(point_pos);
				vertex_num++;
			}
			// parse the faces
			if (tag == "f")
			{
				Face face;
				face.p0_pos = modelPoints[a];
				face.p1_pos = modelPoints[b];
				face.p2_pos = modelPoints[c];
				polyBunny.push_back(face);
				face_num++;
			}
		}
	}
	cout << "vertex_num: " << vertex_num << endl;
	cout << "face_num: " << face_num << endl;
	cout << "Load model success." << endl;
}

