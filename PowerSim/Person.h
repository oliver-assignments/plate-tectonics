#ifndef Person_H
#define Person_H

#include "House.h"

enum Occupation
{
	FARMER,ARTISAN
};

class Person
{
public:
	Person(int myId,
		   int myStrength,int myIntelligence, 
		   
		   int myGeneration, 
		   
		   int myForeignX,int myForeignY,

		   int myProvinceX,int myProvinceY,
		   int myX,int myY)
	{
		power = 10;
		strength = myStrength;
		intelligence = myIntelligence;

		province_x = myProvinceX;
		province_y = myProvinceY;

		position_x = myX;
		position_y = myY;

		foreign_x =myForeignX;
		foreign_y=myForeignY;

		hunger = 10;
		food_carried=30;

		occupation = FARMER;

		generation = myGeneration;

		home = NULL;

		dead =false;
		id= myId;
	};

	House* home;

	int power;

	int strength;
	int intelligence;

	int hunger;
	int food_carried;

	bool dead;
	
	int position_x;
	int position_y;

	int province_x;
	int province_y;

	int foreign_x;
	int foreign_y;

	Occupation occupation;

	int generation;
	int id;
};


#endif