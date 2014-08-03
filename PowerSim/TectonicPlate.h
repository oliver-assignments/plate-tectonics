#ifndef Tectonic_Plate_H
#define Tectonic_Plate_H

class Province;
#include "utility.h"

class TectonicPlate
{
public:
	TectonicPlate()
	{
		x_velocity = -9999;
		y_velocity = -9999;

		plate_number = -9999;
	};
	
	std::vector<Vector2*> provinces_in_plate;
	std::vector<Vector2*> new_plate;
	int x_velocity;
	int y_velocity;

	int plate_number;

	void Move()
	{
		for (int p = 0; p < provinces_in_plate.size(); p++)
		{
			provinces_in_plate[p]->x+=x_velocity;
			provinces_in_plate[p]->y+=y_velocity;
		}
	};
};

#endif