#ifndef Province_H
#define Province_H

#include "Vector2.h"
#include <unordered_map>
class Person;

#define GRASSLAND_ARABILITY 60
#define DESERT_ARABILITY 60
#define JUNGLE_ARABILITY 60
#define FOREST_ARABILITY 60
#define TUNDRA_ARABILITY 60
#define ALPINE_ARABILITY 60

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
		arability = 50;
		biome = WATER;
		food_in_province=0;
	};
	int id;
	Vector2 getCenter()
	{
		return Vector2((p0->x+p1->x+p2->x+p3->x)/4,(p0->y+p1->y+p2->y+p3->y)/4);
	};

	Vector2* p0;
	Vector2* p1;
	Vector2* p2;
	Vector2* p3;

	int altitude;
	int arability;//100 is fully vegetable

	int food_in_province;
	std::vector<Person*> people_in_line;

	Biome biome;
	void SetBiome(Biome myBiome)
	{
		biome = myBiome;
		switch(biome)
		{
		case JUNGLE:arability = JUNGLE_ARABILITY;break;
		case FOREST:arability = FOREST_ARABILITY;break;
		case TUNDRA:arability = TUNDRA_ARABILITY;break;
		case WATER:arability = 0;break;
		case ALPINE:arability = ALPINE_ARABILITY;break;
		case DESERT:arability = DESERT_ARABILITY;break;
		}
	};

	std::unordered_map<int,Person*> people_on_province;
	std::unordered_map<int,House*> homes_on_province;

	int province_x,province_y;
};

#endif