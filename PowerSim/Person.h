#ifndef Person_H
#define Person_H

class Person
{
public:
	Person(int myStrength,int myIntelligence, int myGeneration, int myForeignX,int myForeignY,int myX,int myY)
	{
		power = 10;
		strength = myStrength;
		intelligence = myIntelligence;

		position_x = myX;
		position_y = myY;

		foreign_x =myForeignX;
		foreign_y=myForeignY;

		hunger = 0;
		generation = myGeneration;

		dead =false;
	};

	int power;

	int strength;
	int intelligence;

	int position_x;
	int position_y;

	int foreign_x;
	int foreign_y;

	int hunger;

	int generation;

	bool dead;
};

#endif