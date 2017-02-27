# ifndef __LUX_GRID_H__
# define __LUX_GRID_H__

# include "Volume.h"
# include "Types.h"

namespace lux
{

	// convert a float grid to a float volume
	class FloatGridVolume: public Volume<float>
	{
		public:
			FloatGridVolume(FloatGrid::Ptr grid): myFloatGrid(grid)	{}
			~FloatGridVolume()	{}

			const float eval(const Vector& x) const;

		private:
			FloatGrid::Ptr myFloatGrid;
	};

}

# endif

