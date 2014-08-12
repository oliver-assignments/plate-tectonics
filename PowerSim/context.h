#ifndef __WS_WORLD_STATE__
#define __WS_WORLD_STATE__

#include "utility.h"

class Province;
class TectonicPlate;
class Plant;
class Person;
class Animal;
class House;

/*
This code is a snapshot of a world.

With such a compact group of code, one can easily save
and load worlds and progress them.

Functions for querying the worldstate are housed here.
*/

class Context
{
public:
	~Context();

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

	std::vector<std::vector<std::vector<int>>> plates_on_province;
	std::vector<std::vector<bool>> flipped_plates_on_provinces;

	std::vector<Person*> people;
	std::vector<House*> houses;

	std::vector<Plant*> plants;

	std::vector<Animal*> animals;
	
	std::vector<Province*> GetDiamondOfProvinces(int province_x, int province_y, int radius, bool doGetCenter);
	std::vector<Province*> GetSquareOfProvinces(int province_x, int province_y, int radius, bool doGetCenter);
	
	std::vector<Vector2*> GetDiamondOfCoordinates(int province_x, int province_y, int radius);
	std::vector<Vector2*> GetSquareOfCoordinates(int province_x, int province_y, int radius);

	void WrapCoordinates(Vector2* myCoordinate);
	void WrapCoordinates(int* myX,int* myY);

	int SaveWorld();
	Context* LoadWorld();
};


#endif