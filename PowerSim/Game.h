#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_primitives.h>

#include <vector>
#include <thread>
#include <stdlib.h> 
#include <ctime>
#include <mutex>
#include <string>
#include <algorithm>  

#include "Person.h"
#include "Resource.h"
#include "UIState.h"
#include "ColorContext.h"
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

enum GameState
{
	Menu, Ingame
};
enum IngameState
{
	TERRAIN, 
	PLATE_TECTONICS, 
	PLANT_AND_ANIMAL, 
	HUMAN, 
};

class Game
{
public:
	//Initialize//
	Game();
	void Initialize();
	void InitializeAllegro();
	void InitializeGame();
	void CreateWorld();

	std::string CreateName(int myNumberLetters);

	void CreateProvinces();
	void CreateWater();
	void ResolveWaterInProvince(Province* prov);
	void CreateGrassland();
	void CreateForests();
	void CreateDeserts();
	void CreateEquator();
	void CreateRivers();

	void CreateTectonicPlates();

	void CreatePlants();
	void CreateAnimals();

	void CreateResources(int myNumber);

	void CreatePeople(int myNumberClusters,int myPeoplePerCluster, int myForeignRadius);

	//LoadContent
	void LoadContent();
	void DefineColors();

	//Update//
	void Update();
	void TakeInput();
	void RunTectonics();
	void RunHumans();
	void ProcessPeople();

	//AI
	void ProcessPersonAI(Person* person);
	void SeekDominion(Person* person);
	void SeekFood(Person* person);
	void BuildHome(Person* person);
	void Sleep(Person* person);
	void BuildResources(Person* person);
	void SeekInteraction(Person* person);
	void MoveRandomDirection(Person* person);
	void Game::MoveToCoordinates(Person* person, int x,int y);

	std::vector<Province*> Game::GetSquareOfProvinces(int province_x, int province_y, int radius,bool vertical_wrap,bool horizontal_wrap,bool doGetCenter);
	std::vector<Province*> GetBlobOfProvinces(int x, int y, int radius,bool vertical_wrap,bool horizontal_wrap,bool doGetCenter);
	std::vector<Vector2*> GetBlobOfCoordinates(int province_x, int province_y, int radius);
	void GetInLineForFood(int x,int y,Person* myPerson);
	void DivvyUpFood();
	void UpdateProvinceFood();
	std::vector<Province*> GetAllAdjacentProvinces(int x,int y);
	std::vector<Province*> GetFourAdjacentProvinces(int x,int y);

	void UpdatePersonPositionToProvince(Person* person);

	//Draw
	void Draw();
	void DrawProvinces();
	void DrawRivers();
	void DrawPeople();
	void DrawResources();

	void DrawHouses();
	void CalculateVertexColor(int x, int y,ALLEGRO_VERTEX* myVertices);
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

	void DrawDate();
	void DrawPopulation();

	//Unload
	void FreeMemory();

	long int PMrand();

private:
	//Management//
	//			//
	boolean done;

	GameState currentGameState;
	IngameState currentIngameState;

	double FPS;
	double total_frames;
	double previous_tick_timestamp;
	double elapsed_time;
	double old_time;

	double dt;
	double accumulator;

	ALLEGRO_KEYBOARD_STATE new_keyboard_state,old_keyboard_state;
	ALLEGRO_DISPLAY* display;
	int screen_width;
	int screen_height;
	int screen_game_width;
	int screen_game_height;

	bool draw_every_hour;

	//Game//
	//	  //

	//Life
	int hours_in_day;
	int days_in_year;

	int current_hour;
	int current_day;
	int current_year;

	//Tectonic Plates
	std::vector<TectonicPlate*> listOfTectonicPlates; 
	std::vector<Vector2*> currentPlateCollisions;
	std::vector<Vector2*> oldPlateCollisions;

	//Provinces
	bool province_jiggle;

	//Water Resolution
	std::vector<Province*> province_water_unresolved;
	int times_drawn;
	int number_times_resolved;
	int highest_unresolved;
	std::vector<std::vector<Province*>> provinces;
	std::vector<Vector2*> province_vertices;
	std::vector<Province*> provinces_with_hungry_people;
	std::vector<Vector2*> provinces_with_water;
	int province_width;
	int province_height;
	int provinces_num_columns;
	int provinces_num_rows;

	int province_jiggle_width;
	int province_jiggle_height;

	bool provinces_drawn;
	bool color_province_blending;

	int arability_max;
	int province_max_altitude;
	int province_max_depth;

	int province_id;

	//Rivers
	std::vector<std::vector<Vector2*>> rivers_points;

	//People
	std::vector<Person*> people;
	int player_id;

	//Houses
	int house_id;
	std::vector<int*> color_province_vertex_average;

	//Resources
	std::vector<std::string> consonants;
	std::vector<std::string> vowels;

	std::vector<Resource*> resources;

	bool resources_drawn;

	//Color Relative poitns
	int power_highest_historical;
	int power_highest_current;
	Person* power_highest_person;
	ColorContext power_context;

	int strength_highest_historical;
	int strength_highest_current;
	Person* strength_highest_person;
	ColorContext strength_context;

	int intelligence_highest_historical;
	int intelligence_highest_current;
	Person* intelligence_highest_person;
	ColorContext intelligence_context;

	int hunger_death_level;
	int hunger_seek_level;

	int foreign_max;

	int generation_youngest;

	//Definiting Colors
	UIState ui_state;

	double color_base_value ;

	int color_text[3];
	ALLEGRO_FONT* arial24;
	ALLEGRO_FONT* arial16;
	ALLEGRO_FONT* arial12;
	ALLEGRO_FONT* arial8;

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