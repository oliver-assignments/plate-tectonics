#ifndef __WS_WORLD_STATE__
#define __WS_WORLD_STATE__

#include "utility.h"

class Province;
class TectonicPlate;

/*
This code is a snapshot of a world.

With such a compact group of code, one can easily save
and load worlds and progress them.

Functions for querying the worldstate are housed here.
*/

class Context
{
public:
	Context()
	{
		current_hour=0;
		current_day=0;
		current_year=0;
	};
	~Context();

	std::string world_name;

	int screen_width;
	int screen_height;

	int current_hour;
	int current_day;
	int current_year;

	std::vector<std::vector<Province*>> provinces;
	int province_width;
	int province_height;
	int world_width;
	int world_height;

	std::vector<TectonicPlate*> tectonic_plates; 
	std::vector<std::vector<int>> asthenosphere_heat_map;

	std::vector<std::vector<std::vector<int>>> new_plates_on_province;
	std::vector<std::vector<std::vector<int>>> old_plates_on_province;
	
	std::vector<Province*> GetDiamondOfProvinces(int province_x, int province_y, int radius, bool doGetCenter);
	std::vector<Province*> GetSquareOfProvinces(int province_x, int province_y, int radius, bool doGetCenter);
	std::vector<Vector2*> GetAxisOfCoordinates(int province_x, int province_y, int radius,bool doGetCenter);
	std::vector<Vector2*> GetDiamondOfCoordinates(int province_x, int province_y, int radius,bool doGetCenter);
	std::vector<Vector2*> GetSquareOfCoordinates(int province_x, int province_y, int radius,bool doGetCenter);

	void WrapCoordinates(Vector2* myCoordinate);
	void WrapCoordinates(int* myX,int* myY);
};


#endif