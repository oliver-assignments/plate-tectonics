#include "context.h"

void Context::WrapCoordinates(int* myX,int* myY)
{
	if(*myY <0)
	{
		*myX -= (*myX-(world_width/2))*2;
		if(*myY < 0)
		{
			*myY = abs(*myY) - 1;
		}
	}
	if(*myY >=world_height)
	{
		*myX -= (*myX-(world_width/2))*2;
		if(*myY >=world_height)
			*myY -= *myY - world_height + *myY - world_height+1;
	}
	while(*myX <0)
	{
		*myX = world_width + *myX;
	}
	while(*myX >= world_width)
	{
		*myX -= world_width;
	}
};

void Context::WrapCoordinates(Vector2* myCoordinate)
	{
		if(myCoordinate->y <0)
		{
			myCoordinate->x -= (myCoordinate->x-(world_width/2))*2;
			myCoordinate->y =abs(myCoordinate->y);
		}

		if(myCoordinate->y >=world_height)
		{
			myCoordinate->x -= (myCoordinate->x-(world_width/2))*2;

			if(myCoordinate->y >=world_height)
				myCoordinate->y -= myCoordinate->y - world_height + myCoordinate->y - world_height +1;
		}
		while(myCoordinate->x <0)
		{
			myCoordinate->x += world_width + myCoordinate->x;
		}
		while(myCoordinate->x >=world_width)
		{
			myCoordinate->x -= world_width;
		}

	};

	int Context::SaveWorld()
	{
		std::vector<std::string> lines_of_output;
		//Process current things into strings

		//Provinces
		//Write the dimensions of the world x by y
		//Write the number of provinces
		//Make a for loop that creates a province
		//Altitude
		//WaterDepth
		//X,Y position

		//Tectonics


	};

	Context* Context::LoadWorld()
	{
		//Read settings,
		//if it says NEW then make a new one
		//if it says a file folder then look for that folder
		//in that folder look for a save file

		//Parse save file into a context
		//Return it
	};