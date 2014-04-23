#include "Game.h"

Game::Game()
{
	Initialize();
};

void Game::Initialize()
{
	screen_width = 1350;
	screen_height = 690;

	done= false;
	FPS=0;
	total_frames=0;
	previous_tick_timestamp =0;
	elapsed_time=0;
	old_time=0;

	dt = .05;
	accumulator = 0.0;


	InitializeGame();
	InitializeAllegro();
	LoadContent();

	Update();

};

void Game::InitializeAllegro()
{
	al_init(); //allegro-5.0.10-monolith-md-debug.lib

	//Anti Aliasing
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

	//Creating screen
	display = al_create_display(screen_width,screen_height);
	al_set_window_position(display,0,0);
	al_set_window_title(display,"Allegro Template");

	//Initializing Addons
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(10);
};

void Game::InitializeGame()
{
	ui_state = POWER;

	//Setuping up our values
	hours_in_day =24;
	days_in_year=365;

	current_hour=0;
	current_day=0;
	current_year=0;

	power_highest_historical=1000;
	power_highest_current=10;
	power_context = HISTORY;

	strength_highest_historical =1;
	strength_highest_current=1;
	strength_context= HISTORY;

	intelligence_highest_historical=1;
	intelligence_highest_current=1;
	intelligence_context = HISTORY;

	hunger_death_level = 100;

	foreign_max=100;

	generation_youngest=1;

	//Creating people
	for(int p = 0 ; p < 100;p++)
	{
		Person* person = new Person (10+p,3+p,100+(p*5),100,1);
		people.push_back(person);
	}
};

void Game::LoadContent()
{
	DefineColors();
};



void Game::DefineColors()
{


	color_power = al_map_rgb(255,0,0);
	color_hunger = al_map_rgb(139,69,19);

	color_intelligence= al_map_rgb(100,149,237);
	color_strength= al_map_rgb(255,165,0);

	color_foreign_north= al_map_rgb(255,255,0);
	color_foreign_east= al_map_rgb(204,0,0);
	color_foreign_south= al_map_rgb(51,51,153);
	color_foreign_west= al_map_rgb(51,153,0);
};

void Game::Update()
{
	while(!done)
	{
		//Delta time handling
		double current_timestamp = al_get_time();
		double seconds_since_last_tick = current_timestamp - previous_tick_timestamp;
		if(seconds_since_last_tick > .25)
		{
			seconds_since_last_tick = 0.25;
		}
		previous_tick_timestamp = current_timestamp;
		accumulator+=seconds_since_last_tick;

		//Do this before drawing
		while ( accumulator >= dt )
		{
			TakeInput();


			accumulator -= dt;
		}
		Draw();
	}
};

void Game::TakeInput()
{
	al_get_keyboard_state(&new_keyboard_state);

	//ESCAPE
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_ESCAPE))
	{
		done = true;
	}

	//WASD
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_W))
	{

	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_D))
	{

	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_S))
	{

	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_A))
	{

	}

	//ARROW KEYS
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_UP))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_UP))
		{

		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_RIGHT))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_RIGHT))
		{

		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_DOWN))
	{

	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_LEFT))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_LEFT))
		{

		}
	}

	old_keyboard_state = new_keyboard_state;
};

void Game::Draw()
{
	DrawPeople();

	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));
};

void Game::DrawPeople()
{
	for(std::vector<Person*>::size_type i = 0; i != people.size(); i++) 
	{
		ALLEGRO_COLOR person_color;

		if(ui_state == POWER)
		{
			if(power_context = HISTORY)
			{
				person_color = al_map_rgb(color_power.r*(people[i]->power/power_highest_historical),0,0);
			}
			else//CURRENT
			{
				person_color = al_map_rgb(color_power.r*(people[i]->power/power_highest_current),0,0);
			}
		}
		else if(ui_state == HUNGER)
		{
			person_color = al_map_rgb(
				color_hunger.r*(people[i]->hunger/hunger_death_level * color_hunger.r),
				color_hunger.g*(people[i]->hunger/hunger_death_level * color_hunger.g),
				color_hunger.b*(people[i]->hunger/hunger_death_level * color_hunger.b));
		}
		else if(ui_state == STRENGTH)
		{
			if(strength_context = HISTORY)
			{
				person_color = al_map_rgb(
					color_strength.r*(people[i]->strength/strength_highest_historical * color_strength.r),
					color_strength.g*(people[i]->strength/strength_highest_historical * color_strength.g),
					color_strength.b*(people[i]->strength/strength_highest_historical * color_strength.b));
			}
			else//CURRENT
			{
				person_color = al_map_rgb(
					color_strength.r*(people[i]->strength/strength_highest_current * color_strength.r),
					color_strength.g*(people[i]->strength/strength_highest_current * color_strength.g),
					color_strength.b*(people[i]->strength/strength_highest_current * color_strength.b));
			}
		}
		else if(ui_state == INTELLIGENCE)
		{
			if(intelligence_context = HISTORY)
			{
				person_color = al_map_rgb(
					color_intelligence.r*(people[i]->intelligence/intelligence_highest_historical * color_intelligence.r),
					color_intelligence.g*(people[i]->intelligence/intelligence_highest_historical * color_intelligence.g),
					color_intelligence.b*(people[i]->intelligence/intelligence_highest_historical * color_intelligence.b));
			}
			else//CURRENT
			{
				person_color = al_map_rgb(
					color_intelligence.r*(people[i]->intelligence/intelligence_highest_current * color_intelligence.r),
					color_intelligence.g*(people[i]->intelligence/intelligence_highest_current * color_intelligence.g),
					color_intelligence.b*(people[i]->intelligence/intelligence_highest_current * color_intelligence.b));
			}
		}
		else if(ui_state == FOREIGN)
		{
			if(people[i]->foreign_x>=0 && people[i]->foreign_y>=0)
			{
				//first quadrant
				person_color = al_map_rgb(
					color_intelligence.r*((people[i]->foreign_x/foreign_max * color_foreign_east.r)+(people[i]->foreign_y/foreign_max * color_foreign_north.r) ),
					color_intelligence.g*((people[i]->foreign_x/foreign_max * color_foreign_east.g)+(people[i]->foreign_y/foreign_max * color_foreign_north.g)),
					color_intelligence.b*((people[i]->foreign_x/foreign_max * color_foreign_east.b)+(people[i]->foreign_y/foreign_max * color_foreign_north.b)));
			}
			else if(people[i]->foreign_x>=0 && people[i]->foreign_y>=0)
			{
				//second quadrant
				person_color = al_map_rgb(
					color_intelligence.r*((people[i]->foreign_x/-foreign_max * color_foreign_west.r)+(people[i]->foreign_y/foreign_max * color_foreign_north.r) ),
					color_intelligence.g*((people[i]->foreign_x/-foreign_max * color_foreign_west.g)+(people[i]->foreign_y/foreign_max * color_foreign_north.g)),
					color_intelligence.b*((people[i]->foreign_x/-foreign_max * color_foreign_west.b)+(people[i]->foreign_y/foreign_max * color_foreign_north.b)));
			}
			else if(people[i]->foreign_x>=0 && people[i]->foreign_y>=0)
			{
				//third quadrant
				person_color = al_map_rgb(
					color_intelligence.r*((people[i]->foreign_x/-foreign_max * color_foreign_west.r)+(people[i]->foreign_y/-foreign_max * color_foreign_south.r) ),
					color_intelligence.g*((people[i]->foreign_x/-foreign_max * color_foreign_west.g)+(people[i]->foreign_y/-foreign_max * color_foreign_south.g)),
					color_intelligence.b*((people[i]->foreign_x/-foreign_max * color_foreign_west.b)+(people[i]->foreign_y/-foreign_max * color_foreign_south.b)));
			}
			else
			{
				//fourth quadrant
				person_color = al_map_rgb(
					color_intelligence.r*((people[i]->foreign_x/foreign_max * color_foreign_east.r)+(people[i]->foreign_y/-foreign_max * color_foreign_south.r) ),
					color_intelligence.g*((people[i]->foreign_x/foreign_max * color_foreign_east.g)+(people[i]->foreign_y/-foreign_max * color_foreign_south.g)),
					color_intelligence.b*((people[i]->foreign_x/foreign_max * color_foreign_east.b)+(people[i]->foreign_y/-foreign_max * color_foreign_south.b)));
			}
		}
		else //Generation
		{
			person_color = al_map_rgb(
				color_intelligence.r*(people[i]->generation/ generation_youngest * color_generation.r),
				color_intelligence.g*(people[i]->generation/ generation_youngest * color_generation.g),
				color_intelligence.b*(people[i]->generation/ generation_youngest * color_generation.b));
		}

		DrawBlade(people[i]->position_x,people[i]->position_y,person_color.r,person_color.g,person_color.b);
	}
}
void Game::DrawCluster(int x, int y, unsigned char r,unsigned char g,unsigned char b)
{
	al_draw_pixel(x,y,al_map_rgb(r,g,b));
	al_draw_pixel(x+1,y,al_map_rgb(r,g,b));
	al_draw_pixel(x-1,y,al_map_rgb(r,g,b));
	al_draw_pixel(x,y+1,al_map_rgb(r,g,b));
	al_draw_pixel(x,y-1,al_map_rgb(r,g,b));
};
void Game::DrawBlade(int x, int y, unsigned char r,unsigned char g,unsigned char b)
{
	al_draw_pixel(x,y,al_map_rgb(r,g,b));
	al_draw_pixel(x,y-1,al_map_rgb(r,g,b));
	al_draw_pixel(x,y-2,al_map_rgb(r,g,b));
	al_draw_pixel(x,y-3,al_map_rgb(r,g,b));
	al_draw_pixel(x,y-4,al_map_rgb(r,g,b));
};


void Game::FreeMemory()
{

};
