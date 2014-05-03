#ifndef Person_H
#define Person_H

enum Occupation
{
	Farmer,Craftsman
};

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

		occupation = Farmer;

		generation = myGeneration;

		dead =false;
	};

	int power;

	int strength;
	int intelligence;

	int hunger;

	bool dead;
	
	int position_x;
	int position_y;

	int foreign_x;
	int foreign_y;

	Occupation occupation;

	int generation;
};

#endif