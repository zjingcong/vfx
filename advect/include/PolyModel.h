# ifndef __POLYMODEL_H__
# define __POLYMODEL_H__

# include <fstream>
# include <iostream>
# include <string>
# include <sstream>
# include <vector>

# include "Vector.h"
# include "Types.h"

using namespace std;
using namespace lux;

# define myMax(x, y) (x > y ? x : y)
# define myMin(x, y) (x < y ? x : y)

class Point
{
	public:
		Point()	{};
		Point(Vector& position);	// create a point without point normal info
		Point(Vector& position, Vector& n);	// create a point with position and normal
		~Point()	{};

		void setPos(Vector& position)	{pos = position;}
		void setNormal(Vector& n)	{normal = n;}
		Vector getPos()	{return pos;}
		Vector getNormal()	{return normal;}

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
		// get the min signed distance from a point to a face
		float getSignDistance(Vector p);

	private:
		Point p0;
		Point p1;
		Point p2;

		Vector e1;
		Vector e2;
		Vector normal;
};


class PolyModel
{
	public:
		PolyModel();
		~PolyModel()	{}

		// a vector container to store all the model faces
		std::vector<Face> polyFaces;
		// a vector container to store all triangle indices
		std::vector<Vec3I> triIndices;
		// a vector container to store all point positions
		std::vector<Vec3s> polyPoints;

		// model bounding box
		float x_min, y_min, z_min, x_max, y_max, z_max;

		// obj file parser
		// v position0, position1, position2
		// (vn nx, ny, nz)
		// f p0, p1, p2
		void loadObj(string filePath);

	private:
		int vertex_num;
		int face_num;
		int vertex_normal_num;
};

# endif

