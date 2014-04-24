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

#include "Person.h"
#include "UIState.h"
#include "ColorContext.h"


class Game
{
public:
	Game();
	void Initialize();
	void InitializeGame();
	void InitializeAllegro();

	void LoadContent();
	void DefineColors();

	void Update();
	void TakeInput();
	void RunTime();
	void ProcessPeople();

	void Draw();
	void DrawPeople();
	void DrawCluster(int x, int y, unsigned char r,unsigned char g,unsigned char b);
	void DrawBlade(int x, int y, unsigned char r,unsigned char g,unsigned char b);

	void FreeMemory();

private:
	//Management//
	//			//
	boolean done;

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

	//Game//
	//	  //

	//Life
	int hours_in_day;
	int days_in_year;

	int current_hour;
	int current_day;
	int current_year;

	//People
	std::vector<Person*> people;

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

	int foreign_max;

	int generation_youngest;

	//Definiting Colors
	UIState ui_state;

	int color_power[3];
	int color_hunger[3];
	 
	int color_intelligence[3];
	int color_strength[3];
	 
	int color_foreign_north[3];
	int color_foreign_east[3];
	int color_foreign_south[3];
	int color_foreign_west[3];
	
	int color_generation[3];

	/*ALLEGRO_COLOR color_power;
	ALLEGRO_COLOR color_hunger;
	
	ALLEGRO_COLOR color_intelligence;
	ALLEGRO_COLOR color_strength;
	
	ALLEGRO_COLOR color_foreign_north;
	ALLEGRO_COLOR color_foreign_east;
	ALLEGRO_COLOR color_foreign_south;
	ALLEGRO_COLOR color_foreign_west;

	ALLEGRO_COLOR color_generation;*/
};