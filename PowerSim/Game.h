
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

// PERLYL noise

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
	POWER,HUNGER,STRENGTH,INTELLIGENCE,FOREIGN,GENERATION,OCCUPATION

} CensusMode;

typedef enum
{
	CURRENT, HISTORY

} CensusContext;

class Game
{
public:
	//			//
	//Initialize//
	//			//

	//Game();
	void Initialize();
	//void InitializeAllegro();
	//void InitializeVariables();
	Context* CreateWorld();

	void CreateProvinces();
	void CreateContinents();
	void CreateRivers();

	void DefineColors();

	//		//
	//Update//
	//		//
	void Update();
	
	void UpdateDeepestWater();
	void UpdateHighestMountain();

	//Draw
	void Draw();
	void DrawProvinces();
	//void DrawRivers();
	void DrawPeople();
	

	void DrawHouses();
	//void CalculateVertexColor(int x, int y,ALLEGRO_VERTEX* myVertices);
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

	void DrawData();
	void DrawPopulation();
private:
	//Management//
	//			//

	//												//
	//	Consider making a game window and toolbar	//
	//												//
	int screen_game_width;
	int screen_game_height;

	//Game//
	//	  //

	//Life
	int hours_in_day;
	int days_in_year;

	int current_hour;
	int current_day;
	int current_year;

	//Tectonic Plates
	int plate_count;
	std::vector<std::vector<bool>> provinces_has_plate;
	std::vector<std::vector<std::vector<int>>> plates_on_province;
	std::vector<std::vector<int>> provinces_pending_altitude_changes;
	std::vector<std::vector<int>> asthenosphere_heat_map;

	std::vector<TectonicPlate*> tectonic_plates; 
	std::vector<Vector2*> currentPlateCollisions;
	std::vector<Vector2*> oldPlateCollisions;

	//Provinces
	bool province_jiggle;

	std::vector<std::vector<Province*>> provinces;
	std::vector<Vector2*> province_vertices;
	std::vector<Province*> provinces_with_hungry_people;
	std::vector<Vector2*> provinces_with_water;
	int province_width;
	int province_height;
	int world_width;
	int world_height;

	//Water Resolution
	int total_water;
	int total_land;
	std::vector<Province*> province_water_unresolved;
	int times_drawn;

	int province_jiggle_width;
	int province_jiggle_height;

	bool provinces_drawn;
	bool province_borders_drawn;
	bool province_height_drawn;

	int province_highest_altitude;
	int province_deepest_depth;

	int province_id;

	////Rivers
	//std::vector<std::vector<Vector2*>> rivers_points;

	//People
	int player_id;

	//Houses
	int house_id;

	//Color Relative poitns
	int power_highest_historical;
	int power_highest_current;
	Person* power_highest_person;
	CensusContext power_context;

	int strength_highest_historical;
	int strength_highest_current;
	Person* strength_highest_person;
	CensusContext strength_context;

	int intelligence_highest_historical;
	int intelligence_highest_current;
	Person* intelligence_highest_person;
	CensusContext intelligence_context;

	int hunger_death_level;
	int hunger_seek_level;

	int foreign_max;

	int generation_youngest;

	//Definiting Colors
	CensusMode ui_state;

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
