
# include <ctime>

# include "Lighting.h"
# include "omp.h"

using namespace lux;


// ----------------------------------- Class DSMVolume ------------------------------------

const float DSMVolume::eval(const Vector& x) const
{
	float T = 0;
	if (densityVolume.eval(x) > 0)	// don't do this when using openvdb levelsets
	{
		float s_max = (x - light.getPos()).magnitude();
		Vector normal = (light.getPos() - x).unitvector();
		// initialization
		float s = 0;
		// iteration
		while(s < s_max)
		{
			Vector y = x + normal * s;
			float rho = densityVolume.eval(y);

			s += step_size;
			T += rho * step_size;
		}
	}

	return T;
}

// --------------------------- Class LightVolume ----------------------------------------

LightVolume::LightVolume(std::vector<LightSource> lits, Volume<float>& f, Volume<Color>& c, float k, float delta, float s, BBox& bbox): 
	lights(lits), densityVolume(f), matColorVolume(c), K(k), step_size(delta), voxelSize(s), volumeBBox(bbox)
{
	DSMStamping();
}


// stamp T on the grid and convert to a volume for each light
void LightVolume::DSMStamping()
{
	if (!lights.empty())
	{
		int light_num = int(lights.size());
		for (int i = 0; i < light_num; ++i)
		{
			std::cout << "Stamping LightSource " << i << "..." << std::endl;
			LightSource lit = lights[i];
			DSMVolume sc(densityVolume, lit, step_size);
			// stamp T on the grid
			FloatVolumeToGrid scVolume2Grid(sc, voxelSize, volumeBBox, LIGHT_STAMP);
			FloatGrid::Ptr DSMGrid = scVolume2Grid.getVolumeGrid();
			// convert the grid to volume
			FloatGridVolume DSMGridVolume(DSMGrid);
			gridVolume.push_back(DSMGridVolume);
		}
	}
}


const Color LightVolume::eval(const Vector& x) const
{
	Color finalColor;	// initial value is 0
	if (!lights.empty())
	{
		int light_num = lights.size();
		// calculate the final color for all light sources
		for (int i = 0; i < light_num; ++i)
		{
			LightSource lit = lights[i];
			// calculate the color for each light
			Color litColor = lit.getColor();	// light color
			Color matColor = matColorVolume.eval(x);	// material color
			finalColor += (litColor * matColor * exp(-K * gridVolume[i].eval(x)));
		}
	}

	return finalColor;
}

