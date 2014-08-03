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
*/

namespace ws_global
{
	class Context
	{
	public:

	private:
		int current_hour;
		int current_day;
		int current_year;

		std::vector<std::vector<Province*>> provinces;

		std::vector<TectonicPlate*> tectonic_plates; 
		std::vector<std::vector<int>> asthenosphere_heat_map;

		std::vector<Person*> people;
		std::vector<House*> houses;

		std::vector<Plant*> plants;

		std::vector<Animal*> animals;
	};
}

#endif