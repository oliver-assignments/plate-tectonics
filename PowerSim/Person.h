#ifndef Person_H
#define Person_H



class Person
{
public:
	Person(int myStrength,int myIntelligence, int myX,int myY,int myGeneration)
	{
		power = 10;
		strength = myStrength;
		intelligence = myIntelligence;

		position_x = myX;
		position_y = myY;

		foreign_x =0;
		foreign_y=0;

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