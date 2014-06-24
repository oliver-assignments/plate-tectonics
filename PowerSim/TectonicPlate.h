#ifndef Tectonic_Plate_H
#define Tectonic_Plate_H

class Province;
#include <vector>

class TectonicPlate
{
public:
	TectonicPlate(){};
	
	std::vector<Vector2*> provinces_in_plate;
	double x_velocity;// provinces '/. years it takes 
	double y_velocity;

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