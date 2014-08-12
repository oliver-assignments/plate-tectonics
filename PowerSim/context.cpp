#include "context.h"

Context::~Context()
{
	for (int i = 0; i < tectonic_plates.size(); i++)
	{
		delete(tectonic_plates[i]);
	}

	for (int i = 0; i < people.size(); i++)
	{
		delete(people[i]);
	}
	for (int i = 0; i < houses.size(); i++)
	{
		delete(houses[i]);
	}
	for (int i = 0; i < plants.size(); i++)
	{
		delete(plants[i]);
	}
	for (int i = 0; i < animals.size(); i++)
	{
		delete(animals[i]);
	}
};

std::vector<Province*> Context::GetSquareOfProvinces(int province_x, int province_y, int radius,bool doGetCenter)
{
	std::vector<Province*> square;

	for (int x = province_x-radius; x <= province_x+radius; x++)
	{
		for (int y = province_y-radius; y <= province_y+radius; y++)
		{
			if(x == province_x && y == province_y )
			{
				if(doGetCenter)
					square.push_back(provinces[y][x]);
			}
			else
			{
				int wrapped_x = x;
				int wrapped_y = y;

				WrapCoordinates(&wrapped_x,&wrapped_y);

				square.push_back(provinces[wrapped_y][wrapped_x]);
			}
		}
	}

	return square;
};
std::vector<Province*> Context::GetDiamondOfProvinces(int province_x, int province_y, int radius, bool doGetCenter)
{
	std::vector<Province*> blob;

	//Center
	if(doGetCenter)
	{
		if(province_y >=0 && province_y <world_height && province_x >=0 && province_x <world_width)
		{ 
			blob.push_back(provinces[province_y][province_x]);
		}
	}

	//Making circles
	int location_x = 0;
	int location_y = 0;
	for (int r = radius; r > 0; r--)
	{
		location_x = province_x;
		location_y = province_y-r;

		while(location_x != province_x+r)
		{
			location_x++;
			location_y++;

			int wrapped_location_x = location_x;
			int wrapped_location_y = location_y;

			WrapCoordinates(&wrapped_location_x,&wrapped_location_y);

			blob.push_back(provinces[wrapped_location_y][wrapped_location_x]);


		}

		while(location_x != province_x)
		{
			location_x--;
			location_y++;

			int wrapped_location_x = location_x;
			int wrapped_location_y = location_y;

			WrapCoordinates(&wrapped_location_x,&wrapped_location_y);

			blob.push_back(provinces[wrapped_location_y][wrapped_location_x]);

		}

		while(location_x != province_x-r)
		{
			location_x--;
			location_y--;

			int wrapped_location_x = location_x;
			int wrapped_location_y = location_y;

			WrapCoordinates(&wrapped_location_x,&wrapped_location_y);

			blob.push_back(provinces[wrapped_location_y][wrapped_location_x]);


		}

		while(location_y != province_y-r)
		{
			location_x++;
			location_y--;

			int wrapped_location_x = location_x;
			int wrapped_location_y = location_y;

			WrapCoordinates(&wrapped_location_x,&wrapped_location_y);

			blob.push_back(provinces[wrapped_location_y][wrapped_location_x]);
		}
	}

	return blob;
};
std::vector<Vector2*> Context::GetDiamondOfCoordinates(int province_x, int province_y, int radius)
{
	std::vector<Vector2*> blob;

	//Center
	if(province_y >=0 && province_y <world_height && province_x >=0 && province_x <world_width)
	{ 
		blob.push_back(new Vector2(province_x,province_y));
	}

	//Axis
	for (int r = 1; r <= radius; r++)
	{
		if(province_y-r >=0 && province_y-r <world_height)
		{
			int wrapped_x = province_x;
			if(province_x <0){wrapped_x+=world_width;}
			if(province_x >=world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y-r));
		}
		if(province_y >=0 && province_y <world_height)
		{ 
			int wrapped_x = province_x+r;
			if(province_x+r <0){wrapped_x+=world_width;}
			if(province_x+r >=world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y));
		}
		if(province_y+r >=0 && province_y+r <world_height)
		{
			int wrapped_x = province_x;
			if(province_x <0){wrapped_x+=world_width;}
			if(province_x >=world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y+r));
		}
		if(province_y >=0 && province_y <world_height)
		{ 
			int wrapped_x = province_x-r;
			if(province_x-r <0){wrapped_x+=world_width;}
			if(province_x-r >=world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y));
		}
	}

	//Making circles
	int location_x = 0;
	int location_y = 0;
	for (int r = radius; r > 1; r--)
	{
		location_x = province_x;
		location_y = province_y-r;

		while(location_x != province_x+r)
		{
			location_x++;
			location_y++;
			if(location_y >=0 && location_y <world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >=world_width){wrapped_location-=world_width;}

				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
		while(location_x != province_x)
		{
			location_x--;
			location_y++;
			if(location_y >=0 && location_y <world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >=world_width){wrapped_location-=world_width;}

				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
		while(location_x != province_x-r)
		{
			location_x--;
			location_y--;
			if(location_y >=0 && location_y <world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >=world_width){wrapped_location-=world_width;}

				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
		while(location_y != province_y-r)
		{
			location_x++;
			location_y--;
			if(location_y >=0 && location_y <world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >=world_width){wrapped_location-=world_width;}
				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
	}

	return blob;
};

std::vector<Vector2*> Context::GetSquareOfCoordinates(int province_x, int province_y, int radius)
{
	std::vector<Vector2*> blob;

	//Center
	if(province_y >=0 && province_y < world_height && province_x >=0 && province_x <world_width)
	{ 
		blob.push_back(new Vector2(province_x,province_y));
	}

	//Axis
	for (int r = 1; r <= radius; r++)
	{
		if(province_y-r >=0 && province_y-r <world_height)
		{
			int wrapped_x = province_x;
			if(province_x <0){wrapped_x+=world_width;}
			if(province_x >=world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y-r));
		}
		if(province_y >=0 && province_y < world_height)
		{ 
			int wrapped_x = province_x+r;
			if(province_x+r <0){wrapped_x+=world_width;}
			if(province_x+r >=world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y));
		}
		if(province_y+r >=0 && province_y+r <world_height)
		{
			int wrapped_x = province_x;
			if(province_x <0){wrapped_x+=world_width;}
			if(province_x >=world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y+r));
		}
		if(province_y >=0 && province_y <world_height)
		{ 
			int wrapped_x = province_x-r;
			if(province_x-r <0){wrapped_x+=world_width;}
			if(province_x-r >=world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y));
		}
	}

	//Making circles
	int location_x = 0;
	int location_y = 0;
	for (int r = radius; r > 1; r--)
	{
		location_x = province_x;
		location_y = province_y-r;

		while(location_x != province_x+r)
		{
			location_x++;
			location_y++;
			if(location_y >=0 && location_y <world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >=world_width){wrapped_location-=world_width;}

				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
		while(location_x != province_x)
		{
			location_x--;
			location_y++;
			if(location_y >=0 && location_y <world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >=world_width){wrapped_location-=world_width;}

				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
		while(location_x != province_x-r)
		{
			location_x--;
			location_y--;
			if(location_y >=0 && location_y <world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >=world_width){wrapped_location-=world_width;}

				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
		while(location_y != province_y-r)
		{
			location_x++;
			location_y--;
			if(location_y >=0 && location_y <world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >=world_width){wrapped_location-=world_width;}
				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
	}

	return blob;
}

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
			*myY -= *myY -world_height + *myY -world_height+1;
	}
	while(*myX <0)
	{
		*myX =world_width + *myX;
	}
	while(*myX >=world_width)
	{
		*myX -=world_width;
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
				myCoordinate->y -= myCoordinate->y -world_height + myCoordinate->y -world_height +1;
		}
		while(myCoordinate->x <0)
		{
			myCoordinate->x +=world_width + myCoordinate->x;
		}
		while(myCoordinate->x >=world_width)
		{
			myCoordinate->x -=world_width;
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

		return 0;
	};

	Context* Context::LoadWorld()
	{
		//Read settings,
		//if it says NEW then make a new one
		//if it says a file folder then look for that folder
		//in that folder look for a save file

		//Parse save file into a context
		//Return it
		return new Context();
	};