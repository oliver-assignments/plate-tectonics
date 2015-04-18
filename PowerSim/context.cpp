#include "context.h"

Context::~Context()
{
	for (int i = 0; i < tectonic_plates.size(); i++)
	{
		delete(tectonic_plates[i]);
	}
};

std::vector<Province*> Context::GetSquareOfProvinces(int province_x, int province_y, int radius,bool doGetCenter)
{
	std::vector<Province*> square;

	std::vector<Vector2*> coordinates = GetSquareOfCoordinates(province_x,province_y,radius,doGetCenter);
	for (int i = 0; i < coordinates.size(); i++)
	{
		square.push_back(provinces[coordinates[i]->y][coordinates[i]->x]);
	}

	return square;
};
std::vector<Province*> Context::GetDiamondOfProvinces(int province_x, int province_y, int radius, bool doGetCenter)
{
	std::vector<Province*> blob;

	std::vector<Vector2*> coordinates = GetDiamondOfCoordinates(province_x,province_y,radius,doGetCenter);
	for (int i = 0; i < coordinates.size(); i++)
	{
		blob.push_back(provinces[coordinates[i]->y][coordinates[i]->x]);
	}

	return blob;
};

std::vector<Vector2*> Context::GetAxisOfCoordinates(int province_x, int province_y, int radius,bool doGetCenter)
{
	WrapCoordinates(&province_x,&province_y);

	std::vector<Vector2*> blob;

	if(doGetCenter)
	{
		blob.push_back(new Vector2(province_x,province_y));
	}

	//Axis
	for (int r = 1; r <= radius; r++)
	{
		{
			int wrapped_x = province_x;
			int wrapped_y = province_y-r;
			WrapCoordinates(&wrapped_x,&wrapped_y);
			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
		{ 
			int wrapped_x = province_x+r;
			int wrapped_y = province_y;

			WrapCoordinates(&wrapped_x,&wrapped_y);
			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
		{
			int wrapped_x = province_x;
			int wrapped_y = province_y+r;

			WrapCoordinates(&wrapped_x,&wrapped_y);
			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
		{ 
			int wrapped_x = province_x-r;
			int wrapped_y = province_y;

			WrapCoordinates(&wrapped_x,&wrapped_y);
			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
	}

	return blob;
};
std::vector<Vector2*> Context::GetDiamondOfCoordinates(int province_x, int province_y, int radius,bool doGetCenter)
{
	std::vector<Vector2*> blob;

	WrapCoordinates(&province_x,&province_y);

	//Center
	if(doGetCenter)
	{
		blob.push_back(new Vector2(province_x,province_y));
	}

	// ADD AXIS
	std::vector<Vector2*> axis = GetAxisOfCoordinates( province_x,  province_y,  radius,false);
	for (int i = 0; i < axis.size(); i++)
	{
		blob.push_back(axis[i]);
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

			int wrapped_x= location_x;
			int wrapped_y = location_y;
			WrapCoordinates(&wrapped_x,&wrapped_y);
			blob.push_back(new Vector2(wrapped_x,wrapped_y));

		}
		while(location_x != province_x)
		{
			location_x--;
			location_y++;

			int wrapped_x= location_x;
			int wrapped_y = location_y;
			WrapCoordinates(&wrapped_x,&wrapped_y);
			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
		while(location_x != province_x-r)
		{
			location_x--;
			location_y--;
			
			int wrapped_x= location_x;
			int wrapped_y = location_y;
			WrapCoordinates(&wrapped_x,&wrapped_y);
			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
		while(location_y != province_y-r)
		{
			location_x++;
			location_y--;
			
			int wrapped_x= location_x;
			int wrapped_y = location_y;
			WrapCoordinates(&wrapped_x,&wrapped_y);
			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
	}

	return blob;
};
std::vector<Vector2*> Context::GetSquareOfCoordinates(int province_x, int province_y, int radius,bool doGetCenter)
{
	std::vector<Vector2*> blob;

	WrapCoordinates(&province_x,&province_y);

	//Center
	if(doGetCenter)
	{
		blob.push_back(new Vector2(province_x,province_y));
	}

	// ADD AXIS
	std::vector<Vector2*> axis = GetAxisOfCoordinates( province_x,  province_y,  radius,false);
	for (int i = 0; i < axis.size(); i++)
	{
		blob.push_back(axis[i]);
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

			int wrapped_x = location_x;
			int wrapped_y = location_y;
			WrapCoordinates(&wrapped_x,&wrapped_y);

			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
		while(location_x != province_x)
		{
			location_x--;
			location_y++;

			int wrapped_x = location_x;
			int wrapped_y = location_y;
			WrapCoordinates(&wrapped_x,&wrapped_y);

			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
		while(location_x != province_x-r)
		{
			location_x--;
			location_y--;
			int wrapped_x = location_x;
			int wrapped_y = location_y;
			WrapCoordinates(&wrapped_x,&wrapped_y);

			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
		while(location_y != province_y-r)
		{
			location_x++;
			location_y--;
			int wrapped_x = location_x;
			int wrapped_y = location_y;
			WrapCoordinates(&wrapped_x,&wrapped_y);

			blob.push_back(new Vector2(wrapped_x,wrapped_y));
		}
	}

	return blob;
}

void Context::WrapCoordinates(int* myX,int* myY)
{
	/*if(*myY <0)
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
	}*/
	while(*myY <0)
	{
		*myY =world_height + *myY;
	}
	while(*myY >=world_height)
	{
		*myY -=world_height;
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
	/*if(myCoordinate->y <0)
	{
	myCoordinate->x -= (myCoordinate->x-(world_width/2))*2;
	myCoordinate->y =abs(myCoordinate->y);
	}

	if(myCoordinate->y >=world_height)
	{
	myCoordinate->x -= (myCoordinate->x-(world_width/2))*2;

	if(myCoordinate->y >=world_height)
	myCoordinate->y -= myCoordinate->y -world_height + myCoordinate->y -world_height +1;
	}*/
	while(myCoordinate->y <0)
	{
		myCoordinate->y +=world_height + myCoordinate->y;
	}
	while(myCoordinate->y >=world_width)
	{
		myCoordinate->y -=world_height;
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