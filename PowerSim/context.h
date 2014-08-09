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
	int current_hour;
	int current_day;
	int current_year;

	std::vector<std::vector<Province*>> provinces;
	int world_width;
	int world_height;

	std::vector<TectonicPlate*> tectonic_plates; 
	std::vector<std::vector<int>> asthenosphere_heat_map;

	std::vector<Person*> people;
	std::vector<House*> houses;

	std::vector<Plant*> plants;

	std::vector<Animal*> animals;

	std::vector<Province*> GetSquareOfProvinces(int province_x, int province_y, int radius, bool doGetCenter);
	std::vector<Province*> GetDiamondOfProvinces(int province_x, int province_y, int radius, bool doGetCenter);
	std::vector<Vector2*> GetBlobOfCoordinates(int province_x, int province_y, int radius);

	void WrapCoordinates(Vector2* myCoordinate);
	void WrapCoordinates(int* myX,int* myY);

	void WrapCoordinates(Vector2* myCoordinate);

	void WrapCoordinates(int* myX,int* myY);

	int SaveWorld();

	Context* LoadWorld();

};


#endif