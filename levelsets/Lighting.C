
# include "Lighting.h"
# include "Grid.h"

using namespace lux;


// ------------------------- Class SingleScatterVolume -----------------------------------

const float SingleScatterVolume::eval(const Vector& x) const
{
	float s_max = (x - light.getPos()).magnitude();
	Vector normal = (light.getPos() - x).unitvector();
	// Vector normal = light.getNormal(x);
	// initialization
	float T = 0;
	float s = 0;
	// iteration
	while (s <= s_max)
	{
		Vector y = x + normal * s;
		float rho = densityVolume.eval(y);
		T += rho * step_size;
		s += step_size;
	}

	return T;
}

// --------------------------- Class LightVolume ----------------------------------------

LightVolume::LightVolume(std::vector<LightSource> lits, Volume<float>& f, float k, float delta, float s, BBox& bbox): 
	lights(lits), densityVolume(f), K(k), step_size(delta), voxelSize(s), volumeBBox(bbox)
{
	singleScatterStamping();
}


// stamp T on the grid and convert to a volume for each light
void LightVolume::singleScatterStamping()
{
	if (!lights.empty())
	{
		int light_num = lights.size();
		for (int i = 0; i < light_num; ++i)
		{
			LightSource lit = lights[i];
			SingleScatterVolume sc(densityVolume, lit, step_size);
			// stamp T on the grid
			FloatVolumeToGrid scVolume2Grid(sc, voxelSize, volumeBBox);
			FloatGrid::Ptr singleScatterGrid = scVolume2Grid.getVolumeGrid();
			// convert the grid to volume
			FloatGridVolume singleScatterGridVolume(singleScatterGrid);
			gridVolume.push_back(singleScatterGridVolume);
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
			Color litColor = lit.getColor();
			finalColor += (litColor * exp(-K * gridVolume[i].eval(x)));
		}
	}

	return finalColor;
}

