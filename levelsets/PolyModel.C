
# include "PolyModel.h"
# include <iostream>

using namespace std;

// -------------------------------- Point Class ------------------------------------------

Point::Point(Vector& position)
{
	setPos(position);
}


Point::Point(Vector& position, Vector& n)
{
	setPos(position);
	setNormal(n);
}

// ---------------------------------------------------------------------------------------


// -------------------------------- Face Class -------------------------------------------

Face::Face(Point P0, Point P1, Point P2)
{
	setPoints(P0, P1, P2);
}


void Face::setPoints(Point P0, Point P1, Point P2)
{
	p0 = P0;
	p1 = P1;
	p2 = P2;

	// calculate face normal
	e1 = p1.getPos() - p0.getPos();
	e2 = p2.getPos() - p0.getPos();
	normal = (e2 ^ e1).unitvector();
}


Point Face::getPoint(int point_id)
{
	switch (point_id)
	{
		case 0:
			return p0;
		case 1:
			return p1;
		case 2:
			return p2;
		default:
			return p0;
	}
}


float Face::getSignDistance(Vector x)
{
	Vector pos0 = p0.getPos();
	float a, b, t, d;
	int isInside;	// inside: 1, outside: -1
	
	t = normal * (pos0 - x);	// t < 0 outside
	isInside = (t < 0) ? (-1) : 1;
	a = e2 * (normal ^ (x - pos0)) / (e2 * (normal ^ e1));
	b = e1 * (normal ^ (x - pos0)) / (e1 * (normal ^ e2));
	// case 1
	if (a <= 1 && a >= 0 && b <= 1 && b >= 0 && (a + b) <= 1 && (a + b) >= 0)	{return t;}
	// case 2
	else
	{
		a = (e1 * (x - pos0)) / (e1.magnitude() * e1.magnitude());
		if (a <= 1 && a >= 0)
		{
			d = (x - pos0 - a * e1).magnitude();
			return d * isInside;
		}
		else
		{
			b = (e2 * (x - pos0)) / (e2.magnitude() * e2.magnitude());
			if (b <= 1 && b >= 0)
			{
				d = (x - pos0 - b * e2).magnitude();
				return d * isInside;
			}
			else
			{
				Vector e3 = e2 - e1;
				Vector pos1 = p1.getPos();
				float c = (e3 * (x - pos1)) / (e3.magnitude() * e3.magnitude());
				if (c <= 1 && c >= 0)
				{
					d = (x - pos1 - c * e3).magnitude();
					return d * isInside;
				}
				// case 3
				else
				{
					float d0 = (x - pos0).magnitude();
					float d1 = (x - p1.getPos()).magnitude();
					float d2 = (x - p2.getPos()).magnitude();
					float tmp = myMin(d0, d1);
					float d_myMin = myMin(tmp, d2);
					return d_myMin * isInside;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------------------

// ----------------------------- PolyModel Class ------------------------------------------

// obj file parser
// v position0, position1, position2
// f p0, p1, p2
void PolyModel::loadObj(string filePath)
{
	cout << "Load model " << filePath << "..." << endl;
	// load model file
	const char* file_path = filePath.c_str();
	ifstream modelFile(file_path);

	std::vector<Point> modelPoints;	// a vector container to store model points
	string line;

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
				Point modelPoint(point_pos);
				modelPoints.push_back(modelPoint);
				vertex_num++;
			}
			// parse the vertex normal
			if (tag == "vn")
			{
				Vector point_norm(a, b, c);
				modelPoints[vertex_normal_num].setNormal(point_norm);
				vertex_normal_num++;
			}
			// parse the faces
			if (tag == "f")
			{
				Face face(modelPoints[a - 1], modelPoints[b - 1], modelPoints[c - 1]);	// the point index 0 = first point
				polyFaces.push_back(face);
				face_num++;
			}
		}
	}
	cout << "vertex_num: " << vertex_num << endl;
	cout << "face_num: " << face_num << endl;
	cout << "vertex_normal_num: " << vertex_normal_num << endl;
	cout << "Load model success." << endl;
}

// ----------------------------------------------------------------------------------------

