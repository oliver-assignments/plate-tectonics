#ifndef House_H
#define House_H

class Person;

class House
{
public:
	House(int myId,Person* myOwner,int myProvinceX,int myProvinceY,int myX,int myY)
	{
		owner = myOwner;

		province_x = myProvinceX;
		province_y = myProvinceY;
		position_x = myX;
		position_y = myY;

		abandoned = false;
	};
	Person* owner;

	bool abandoned;

	int position_x;
	int position_y;

	int province_x;
	int province_y;

	int food_stored;
	int date_last_visited;
};

#endif