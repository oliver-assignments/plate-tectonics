#ifndef __WS_GAME__
#define __WS_GAME__

#include "settings.h"
#include "allegro_engine.h"
#include "utility.h"

#include "context.h"
#include "people.h"
#include "plants.h"
#include "animals.h"
#include "tectonic.h"

#include "Person.h"
#include "Resource.h"
#include "Province.h"
#include "Plant.h"
#include "TectonicPlate.h"

/*
Notes

Food production thought order
Favorite Food(Tastiest) and Highest Payoff
Greatest certainty least effort

Some noticeable things about fruit:
Size
Fleshiness
Bitterness
Oiliness
Length of seed

Somethign must encourage a blend of hunting and farming

Hunter Gatherer to Farmer
Plants like peas and wheat had low catalyst, were tasty, and not difficult, easy to domesticate
This allowed for quick transition

Resource
Tastiness
Poision
Difficulty to gather
Difficulty to process
Edible
Lethality of gather

Wild yield

Work catalyst

Ease of domestication
Domestication yield Yield

Latitude of growth
Grows in biome

Speed of spread

Domesticating Animals
Meat, milk, cloths, travel and plows

Technology
Acceptance of tech
Commericalable
Prestige
Politics and inplace shit
How easy is it to see results
Tecnology spread
See and adopt
The get conquered and replaced
Technological Diffusion
Trade
Espionage
Emigration
War
Technology loss due ot weak spread

Autocatalytic technology

Tech develops fastest in 
larg prductive regions wiht large pops
many potential inventors
competativeness

Variations
Time of food prod
barriers to diffusion such as deserts
human pop size

So food can spread west and east easy, so can tech
*/

enum MapMode
{
	TERRAIN, 
	PLATE_TECTONICS, 
	PLANT,
	ANIMAL,
	HUMAN, 
};

typedef enum 
{
	POWER,
	HUNGER,
	STRENGTH,INTELLIGENCE,
	FOREIGN,
	GENERATION,
	OCCUPATION

} CensusMode;

class Game
{
public:
	//			//
	//Initialize//
	//			//

	//Game();
	void Initialize();
	void InitializeVariables();
	void ProcessSettings();
	void CreateWorld();

	void CreateProvinces();
	void CreateContinents();
	//void CreateRivers();

	//		//
	//Update//
	//		//
	void Update();
	
	void UpdateDeepestWater();
	void UpdateHighestMountain();

	//Draw
	void Draw();

	void DrawProvinces();
	//void CalculateVertexColor(int x, int y, ALLEGRO_VERTEX* myVertices);
	
	//void DrawRivers();
	void DrawPeople();
	
	//Drawing Homes
	void DrawHouses();
	void DrawHouse(int x, int y);
	void DrawHamlet (int x, int y); 
	void DrawVillage(int x, int y);
	void DrawTown(int x, int y);
	void DrawCity(int x, int y);
	void DrawMetropolis(int x, int y); 
	void DrawConurbation(int x, int y);
	void DrawMegalopolis(int x, int y);

	void DrawCluster(int x, int y, unsigned char r,unsigned char g,unsigned char b);
	void DrawBlade(int x, int y, unsigned char r,unsigned char g,unsigned char b);

	Context* context;

private:

	std::vector<Vector2*> province_vertices;
	bool province_jiggle;

	int province_jiggle_width;
	int province_jiggle_height;

	bool province_borders_drawn;
	bool province_height_drawn;

	int province_highest_altitude;
	int province_deepest_depth;

	int province_id;
	int player_id;
	int house_id;

	//Definiting Colors
	CensusMode census_mode;
	MapMode map_mode;

	double color_base_value ;

	int color_text[3];

	int color_resource[3];
	int color_house[3];

	int color_grassland[3];
	int color_forest[3];
	int color_alpine[3];
	int color_desert[3];
	int color_water[3];
	int color_jungle[3];
	int color_tundra[3];

	int color_occupation_farmer[3];
	int color_occupation_artisan[3];

	int color_power[3];
	int color_hunger[3];

	int color_intelligence[3];
	int color_strength[3];

	int color_foreign_north[3];
	int color_foreign_east[3];
	int color_foreign_south[3];
	int color_foreign_west[3];

	int color_generation[3];
};

#endif