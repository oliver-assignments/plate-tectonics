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

class Game
{
public:
	//Initialize//
	Game();
	void Initialize();
	void InitializeGame();
	void CreateWorld();
	void CreateProvinces();
	void CreateResources(int myNumber);
	void CreatePeople(int myNumberClusters,int myPeoplePerCluster, int myForeignRadius);
	void InitializeAllegro();

	//LoadContent
	void LoadContent();
	void DefineColors();

	//Update//
	void Update();
	void TakeInput();
	void RunTime();
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

	void GetInLineForFood(int x,int y,Person* myPerson);
	void DivvyUpFood();
	void UpdateProvinceFood();

	void UpdatePersonPositionToProvince(Person* person);

	//Draw
	void Draw();
	void DrawProvinces();
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
	std::mutex mtx;

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

	bool draw_every_hour;

	//Game//
	//	  //

	//Life
	int hours_in_day;
	int days_in_year;

	int current_hour;
	int current_day;
	int current_year;

	//Provinces
	std::vector<std::vector<Province*>> provinces;
	std::vector<Vector2*> province_vertices;
	std::vector<Province*> provinces_with_hungry_people;
	int province_width;
	int province_height;
	int provinces_num_columns;
	int provinces_num_rows;

	int province_jiggle_width;
	int province_jiggle_height;

	bool provinces_drawn;
	bool color_province_blending;

	int arability_max;

	int province_id;

	//People
	std::vector<Person*> people;
	int player_id;

	//Houses
	std::vector<House*> houses;
	int house_id;
	std::vector<int*> color_province_vertex_average;

	//Resources
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

	int color_resource[3];
	int color_house[3];
	int color_province[3];

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