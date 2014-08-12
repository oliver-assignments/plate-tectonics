#ifndef Tectonic_Plate_H
#define Tectonic_Plate_H

#include "utility.h"

class Province;

class TectonicPlate
{
public:
	TectonicPlate()
	{
		x_velocity = -9999;
		y_velocity = -9999;

		plate_number = -9999;
	};
	~TectonicPlate()
	{
		for (int i = 0; i < provinces_in_plate.size(); i++)
		{
			delete(provinces_in_plate[i]);
		}
		for (int i = 0; i < new_plate.size(); i++)
		{
			delete(new_plate[i]);
		}
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