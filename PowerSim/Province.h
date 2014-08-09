#ifndef Province_H
#define Province_H

#include "Vector2.h"
#include <unordered_map>
class Person;
class Plant;

enum Biome
{
	GRASSLAND,DESERT,JUNGLE,FOREST,TUNDRA,ALPINE,WATER
};

class Province
{
public:
	Province(int myId,int myProvinceX,int myProvinceY,Vector2* myP0,Vector2* myP1,Vector2* myP2,Vector2* myP3)
	{
		id = myId;

		province_x = myProvinceX;
		province_y = myProvinceY;

		p0 = myP0;
		p1 = myP1;
		p2 = myP2;
		p3 = myP3;
		biome = DESERT;
		water_depth = 0;
		food_in_province=0;
		altitude = 0;
	};
	int id;
	Vector2 getCenter()
	{
		return Vector2((p0->x + p1->x + p2->x + p3->x)/4,(p0->y+p1->y+p2->y+p3->y)/4);
	};

	int getLandAndWaterHeight()
	{
		return altitude+ water_depth;
	}
	
	Vector2* p0;
	Vector2* p1;
	Vector2* p2;
	Vector2* p3;

	int water_depth;
	int altitude;//from deepest depths
	int distance_from_equator;//Used for climates
	int arability;

	Biome biome;

	int food_in_province;
	std::vector<Person*> people_in_line;
	std::unordered_map<int,Person*> people_on_province;
	std::unordered_map<int,House*> homes_on_province;
	
	int province_x,province_y;
};

#endif