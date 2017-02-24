# ifndef __POLYMODEL_H__
# define __POLYMODEL_H__

# include <fstream>
# include <iostream>
# include <string>
# include <sstream>
# include <list>
# include <vector>

# include "Vector.h"

using namespace std;
using namespace lux;

# define max(x, y) (x > y ? x : y)
# define min(x, y) (x < y ? x : y)

class Point
{
	public:
		Point()	{};
		Point(Vector& position);	// create a point without point normal info
		Point(Vector& position, Vector& n);	// create a point with position and normal
		~Point()	{};

		void setPos(Vector& position)	{pos = position;}
		void setNormal(Vector& n)	{normal = n;}
		Vector& getPos()	{return pos;}
		Vector& getNormal()	{return normal;}

	private:
		Vector pos;	// point position
		Vector normal;	// point normal
};


// triangle face
class Face
{
	public:
		Face()	{};
		Face(Point P0, Point P1, Point P2);
		~Face()	{};

		void setPoints(Point P0, Point P1, Point P2);
		Point getPoint(int point_id);
		float getSignDistance(Vector p);	// get the min distance from a point to a face

	private:
		Point p0;
		Point p1;
		Point p2;

		Vector e1;
		Vector e2;
		Vector normal;
};


// obj file parser
// v position0, position1, position2
// f p0, p1, p2
void load_obj(string filePath, std::list<Face> polyBunny);

# endif


/*
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
	float a, b, t;
	int isInside;	// inside: 1, outside: -1
	
	t = normal * (pos0 - x);	// t < 0 outside
	isInside = (t < 0) ? (-1) : 1;
	a = e2 * (normal ^ (x - pos0)) / (e2 * (normal ^ e1));
	b = e1 * (normal ^ (x - pos0)) / (e1 * (normal ^ e2));
	if (a <= 1 && a >= 0 && b <= 1 && b >= 0 && (a + b) <= 1 && (a + b) >= 0)	{return t;}
	else
	{
		a = (e1 * (x - pos0)) / (e1.magnitude() * e1.magnitude());
		if (a <= 1 && a >= 0)
		{
			float d = (x - pos0 - a * e1).magnitude();
			return d * isInside;
		}
		else
		{
			float d0 = (x - pos0).magnitude();
			float d1 = (x - p1.getPos()).magnitude();
			float d2 = (x - p2.getPos()).magnitude();
			float tmp = min(d0, d1);
			float d_min = min(tmp, d2);
			return d_min * isInside;
		}
	}
}

// ----------------------------------------------------------------------------------------


// obj file parser
// v position0, position1, position2
// f p0, p1, p2
void load_obj(string filePath, std::list<Face> polyModel)
{
	cout << "Load model " << filePath << "..." << endl;
	// load model file
	const char* file_path = filePath.c_str();
	ifstream modelFile(file_path);

	std::vector<Point> modelPoints;	// a vector container to store model points
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
				Point modelPoint(point_pos);
				modelPoints.push_back(modelPoint);
				vertex_num++;
			}
			// parse the faces
			if (tag == "f")
			{
				Face face(modelPoints[a], modelPoints[b], modelPoints[c]);
				polyModel.push_back(face);
				face_num++;
			}
		}
	}
	cout << "vertex_num: " << vertex_num << endl;
	cout << "face_num: " << face_num << endl;
	cout << "Load model success." << endl;
}
*/
