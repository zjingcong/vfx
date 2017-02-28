
# ifndef __LUX_LIGHTING_H_
# define __LUX_LIGHTING_H_

# include <vector>
# include "math.h"

# include "Vector.h"
# include "Color.h"
# include "Grid.h"
# include "Types.h"


namespace lux
{

	class LightSource
	{
		public:
			LightSource(const Vector& pos, Color c): position(pos), litColor(c)	{}
			~LightSource()	{}

			const Vector& getPos() const	{return position;}
			Color getColor()	{return litColor;}
			Vector getNormal(Vector x)	{return (position - x).unitvector();}

		private:
			Vector position;
			Color litColor;
	};


	class SingleScatterVolume: public Volume<float>
	{
		public:
			SingleScatterVolume(Volume<float>& f, LightSource& lit, float delta): 
				densityVolume(f), light(lit), step_size(delta)	{}
			~SingleScatterVolume()	{}

			const float eval(const Vector& x) const;

		private:
			Volume<float>& densityVolume;
			LightSource light;
			float step_size;
	};


	class LightVolume: public Volume<Color>
	{
		public: 
			LightVolume(std::vector<LightSource> lits, Volume<float>& f, float k, float delta, float s, BBox& bbox);
			~LightVolume()	{}

			const Color eval(const Vector& x) const;

		private:
			Volume<float>& densityVolume;
			float K;
			float step_size;
			float voxelSize;
			BBox volumeBBox;

			std::vector<LightSource> lights;
			std::vector<FloatGridVolume> gridVolume;

			void singleScatterStamping();
	};

}

# endif

